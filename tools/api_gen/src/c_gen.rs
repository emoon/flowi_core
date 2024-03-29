use crate::api_parser::*;
use heck::{ToSnakeCase, ToUpperCamelCase};
use serde::Serialize;
use std::collections::HashMap;
use std::fs::File;
use std::io;
use std::io::{BufWriter, Write};
use std::process::Command;
use tera::{Context, Tera, Value};

pub static C_API_SUFFIX: &str = "Fl";
pub static C_API_SUFIX_FUNCS: &str = "fl";

pub struct Cgen;

#[derive(PartialEq, Clone, Copy, Debug)]
enum Ctx<'a> {
    Yes(&'a str),
    No,
}

#[derive(Default)]
struct FuncArgs {
    func_args: Vec<String>,
    internal_args: Vec<String>,
    call_args: Vec<String>,
    body: String,
    return_value: String,
}

fn arg_line(args: &[String], with_context: Ctx) -> String {
    let mut output = String::with_capacity(256);

    match with_context {
        Ctx::Yes(context_name) => output.push_str(context_name),
        Ctx::No => {}
    }

    for (i, a) in args.iter().enumerate() {
        if i > 0 || with_context != Ctx::No {
            output.push_str(", ")
        }

        output.push_str(a);
    }

    output
}

fn get_args_name(func: &Function) -> String {
    format!("{}{}Args", C_API_SUFFIX, func.name.to_upper_camel_case())
}

fn run_clang_format(filename: &str) {
    Command::new("clang-format")
        .arg("-style=file")
        .arg("-i")
        .arg(filename)
        .output()
        .expect("failed to execute clang-format");
}

impl Cgen {
    fn write_commment<W: Write>(
        f: &mut W,
        comments: &Vec<String>,
        indent: usize,
    ) -> io::Result<()> {
        if !comments.is_empty() {
            for c in comments {
                writeln!(f, "{:indent$}// {}", "", c, indent = indent)?;
            }
        }

        Ok(())
    }

    fn get_enum_name(def_name: &str, entry_name: &str) -> String {
        format!("{}{}_{}", C_API_SUFFIX, def_name, entry_name)
    }

    /// Generate enums in the style of
    ///
    /// typedef enum Fl<EnumName> {
    ///    // Optional comment
    ///    Fl<EnumName>_<EntryName> = <Value>,
    /// } Fl<EnumName>
    ///
    fn generate_enum<W: Write>(f: &mut W, enum_def: &Enum) -> io::Result<()> {
        Self::write_commment(f, &enum_def.doc_comments, 0)?;

        writeln!(f, "typedef enum {}{} {{", C_API_SUFFIX, enum_def.name)?;

        for entry in &enum_def.entries {
            Self::write_commment(f, &entry.doc_comments, 4)?;

            match &entry.value {
                EnumValue::Value(v) => writeln!(
                    f,
                    "    {} = {},",
                    Self::get_enum_name(&enum_def.name, &entry.name),
                    v
                )?,
                EnumValue::OrList(_) => writeln!(f)?,
                _ => (),
            }
        }

        writeln!(f, "}} {}{};\n", C_API_SUFFIX, enum_def.name)
    }

    fn get_variable(var: &Variable, self_type: &str) -> String {
        let mut output = String::with_capacity(256);

        match var.vtype {
            VariableType::None => output.push_str("void"),
            VariableType::SelfType => {
                // hack
                if self_type == "Context*" {
                    output.push_str(&format!("struct {}{}", C_API_SUFFIX, self_type));
                } else {
                    output.push_str(&format!("{}{}", C_API_SUFFIX, self_type));
                }
            }

            VariableType::Reference => panic!("Shouldn't be here"),
            VariableType::Regular => {
                // hack, fix me
                if var.type_name == "Context" {
                    output.push_str("struct FlContext");
                } else if var.pointer && var.is_empty_struct {
                    output.push_str(&format!("struct {}{}", C_API_SUFFIX, var.type_name));
                } else {
                    output.push_str(&format!("{}{}", C_API_SUFFIX, var.type_name));
                }
            }

            VariableType::Enum => output.push_str(&format!("{}{}", C_API_SUFFIX, var.type_name)),
            VariableType::Str => output.push_str("FlString"),
            VariableType::Primitive => output.push_str(&var.get_c_primitive_type()),
        }

        if var.pointer {
            output.push('*');
        }

        output
    }

    /// Output variable for for struct
    fn write_struct_variable<W: Write>(f: &mut W, var: &Variable) -> io::Result<()> {
        Self::write_commment(f, &var.doc_comments, 4)?;

        write!(f, "    {}", Self::get_variable(var, ""))?;

        // for arrays we generate a pointer and a size
        match var.array {
            None => writeln!(f, " {};", var.name),
            Some(ArrayType::Unsized) => {
                writeln!(f, "* {};", var.name)?;
                writeln!(f, "    uint32_t {}_size;", var.name)
            }

            Some(ArrayType::SizedArray(ref size)) => {
                writeln!(f, " {}[{}];", var.name, size)
            }
        }
    }

    /// Generate structs in the style of
    ///
    /// typedef struct Fl<StructName> {
    ///    // Optional comment
    ///    ... entries
    /// } Fl<StructName>
    ///
    fn generate_struct<W: Write>(f: &mut W, sdef: &Struct) -> io::Result<()> {
        Self::write_commment(f, &sdef.doc_comments, 0)?;

        /*
        if sdef.variables.is_empty() && !sdef.functions.is_empty() {
            writeln!(f, "struct {}{}Api;\n", C_API_SUFFIX, sdef.name)?;
        }
        */

        // if we have handle set we just generate it as a i32 instead
        if sdef.has_attribute("Handle") {
            writeln!(f, "typedef uint64_t {}{};\n", C_API_SUFFIX, sdef.name)
        } else if sdef.variables.is_empty() {
            writeln!(f, "struct {}{};\n", C_API_SUFFIX, sdef.name)
        } else {
            writeln!(f, "typedef struct {}{} {{", C_API_SUFFIX, sdef.name)?;

            for var in &sdef.variables {
                Self::write_struct_variable(f, var)?;
            }

            writeln!(f, "}} {}{};\n", C_API_SUFFIX, sdef.name)
        }
    }

    /// Generate function prototypes in the style of

    /// Generate structure and _default macro for functions that has default parameters
    fn generate_default_args_struct<W: Write>(
        f: &mut W,
        name: &str,
        args: &[&Variable],
    ) -> io::Result<()> {
        writeln!(f, "typedef struct {} {{", name)?;

        for var in args {
            Self::write_struct_variable(f, var)?;
        }

        writeln!(f, "}} {};\n", name)?;

        // Generate <StructName>_default macro for default value init of the struct such as
        // #define FoobarArgs_default (FoobarArgs){ .foo = 1, .bar = 0 }

        write!(f, "#define {}_default ({}){{", name, name)?;

        for (i, var) in args.iter().enumerate() {
            if i > 0 {
                write!(f, ",")?;
            }

            match var.vtype {
                VariableType::Enum => write!(
                    f,
                    " .{} = {}_{}",
                    var.name, var.type_name, var.default_value
                )?,
                _ => write!(f, " .{} = {}", var.name, var.default_value)?,
            }
        }

        writeln!(f, ")\n")
    }

    /*
    pub fn generate_render_file(filename: &str, render_commands: &[&String]) -> io::Result<()> {
        let mut f = BufWriter::new(File::create(filename)?);

        writeln!(f, "{}", RENDER_CMD_HEADER)?;

        // We want to generate in this sstyle
        // #define Render_create_texture_cmd(state) \
        //     (FlCreateTexture*)CommandBuffer_alloc_cmd(&state->render_commands, Fl_CreateTexture, sizeof(FlCreateTexture))

        for cmd in render_commands {
            let name = format!("{}{}", C_API_SUFFIX, cmd);
            writeln!(f, "#define Render_{}_cmd(state) \\", cmd.to_snake_case())?;
            writeln!(
                    f,
                    "    ({}*)CommandBuffer_alloc_cmd(&state->render_commands, {}RenderCommand_{}, sizeof({}))\n",
                    name, C_API_SUFFIX, cmd, name
                    )?;
        }

        Ok(())
    }
    */

    /// Generate function prototypes in the style of
    fn generate_function_args(func: &Function, self_name: &str) -> FuncArgs {
        let mut fa = FuncArgs::default();

        fa.call_args.push("api->priv".to_owned());

        for (i, arg) in func.function_args.iter().enumerate() {
            // skip first arg if type is manual or static
            if i == 0 && func.is_type_manual_static() {
                continue;
            }

            // If we have a default value we assemu the rest of of the args are default also
            if !arg.default_value.is_empty() {
                let args = format!("{} args", get_args_name(func));
                fa.func_args.push(args.to_owned());
                fa.internal_args.push(args.to_owned()); // breaks editor coloring if not used
                fa.call_args.push("args".to_owned());
                break;
            }

            match arg.vtype {
                VariableType::Str => {
                    fa.func_args.push(format!("const char* {}", arg.name));
                    fa.body.push_str(&format!(
                        "FlString {}_ = fl_cstr_to_flstring({});",
                        arg.name, arg.name,
                    ));

                    fa.internal_args.push(format!("FlString {}", arg.name));
                    fa.call_args.push(format!("{}_", arg.name));
                }

                _ => match arg.array {
                    None => {
                        let carg = format!("{} {}", Self::get_variable(arg, self_name), arg.name);
                        fa.func_args.push(carg.to_owned());
                        fa.internal_args.push(carg.to_owned());
                        fa.call_args.push(arg.name.to_owned());
                    }

                    Some(ArrayType::Unsized) => {
                        let carg = format!("{}* {}", Self::get_variable(arg, self_name), arg.name);
                        fa.func_args.push(carg.to_owned());
                        fa.internal_args.push(carg.to_owned());
                        fa.call_args.push(arg.name.to_owned());
                        let carg = format!("uint32_t {}_size", arg.name);
                        fa.func_args.push(carg.to_owned());
                        fa.internal_args.push(carg.to_owned());
                        fa.call_args.push(format!("{}_size", arg.name));
                    }

                    Some(ArrayType::SizedArray(ref size)) => {
                        let carg = format!(
                            "{} {}[{}]",
                            Self::get_variable(arg, self_name),
                            arg.name,
                            size
                        );
                        fa.func_args.push(carg.to_owned());
                        fa.internal_args.push(carg.to_owned());
                        fa.call_args.push(arg.name.to_owned());
                    }
                },
            }
        }

        if let Some(ret) = &func.return_val {
            if ret.const_pointer {
                fa.return_value = format!("{}*", Self::get_variable(ret, self_name));
            } else {
                fa.return_value = Self::get_variable(ret, self_name);
            }
        } else {
            fa.return_value = "void".to_owned();
        }

        fa
    }

    fn generate_callback_function<W: Write>(
        f: &mut W,
        func: &Function,
        self_name: &str,
    ) -> io::Result<()> {
        let fa = Self::generate_function_args(func, self_name);
        writeln!(
            f,
            "typedef {} (*{}{})({});",
            fa.return_value,
            C_API_SUFFIX,
            func.name,
            arg_line(&fa.func_args, Ctx::Yes("struct FlContext* ctx"))
        )
    }

    fn generate_function<W: Write>(
        f: &mut W,
        func: &Function,
        self_name: &str,
        with_ctx: Ctx,
    ) -> io::Result<()> {
        let fa = Self::generate_function_args(func, self_name);

        Self::write_commment(f, &func.doc_comments, 0)?;

        let self_name_snake = self_name.to_snake_case();

        // write the implementation func

        #[rustfmt::skip]
        let func_name = format!("{}_{}_{}", C_API_SUFIX_FUNCS, &self_name_snake, func.name);
        let func_name_c = &func_name;

        let arg_offset = usize::from(with_ctx == Ctx::No);

        /*
        if with_ctx == Ctx::No {
            writeln!(
                f,
                "{} {}_impl({});\n",
                fa.return_value,
                &func_name,
                arg_line(&fa.internal_args, with_ctx)
            )?;
        }
        */

        writeln!(
            f,
            "FL_INLINE {} {}({}) {{",
            fa.return_value,
            func_name_c,
            arg_line(&fa.func_args, with_ctx)
        )?;

        if !fa.body.is_empty() {
            writeln!(f, "{}", &fa.body)?;
        }

        let argument_line = arg_line(
            &fa.call_args[arg_offset..],
            Ctx::Yes(&format!("g_flowi_{}_api->priv", self_name_snake)),
        );

        writeln!(f, "#ifdef FLOWI_STATIC")?;

        if fa.return_value != "void" {
            writeln!(f, "return {}_impl({});", func_name, argument_line)?;
        } else {
            writeln!(f, "{}_impl({});", func_name, argument_line)?;
        }

        writeln!(f, "#else")?;

        if fa.return_value != "void" {
            writeln!(
                f,
                "return (g_flowi_{}_api->{})({});",
                self_name_snake, func.name, argument_line
            )?;
        } else {
            writeln!(
                f,
                "(g_flowi_{}_api->{})({});",
                self_name_snake, func.name, argument_line
            )?;
        }

        writeln!(f, "#endif")?;
        writeln!(f, "}}\n")
    }

    fn generate_function_def<W: Write>(
        f: &mut W,
        func: &Function,
        self_name: &str,
        with_ctx: Ctx,
    ) -> io::Result<()> {
        let fa = Self::generate_function_args(func, self_name);

        Self::write_commment(f, &func.doc_comments, 0)?;

        #[rustfmt::skip]
        let func_name = format!("{}_{}_{}", C_API_SUFIX_FUNCS, self_name.to_snake_case(), func.name);

        #[rustfmt::skip]
        writeln!(f, "static {} {}({});\n", fa.return_value, func_name, arg_line(&fa.func_args, with_ctx))?;

        Ok(())
    }

    fn generate_function_dynamic<W: Write>(
        f: &mut W,
        func: &Function,
        self_name: &str,
        with_ctx: Ctx,
    ) -> io::Result<()> {
        let fa = Self::generate_function_args(func, self_name);

        #[rustfmt::skip]
        writeln!(f, "{} (*{})({});", fa.return_value, &func.name, arg_line(&fa.internal_args, with_ctx))?;

        Ok(())
    }

    pub fn generate_main_file(path: &str, api_defs: &[ApiDef]) -> io::Result<()> {
        let filename = format!("{}/application.h", path);

        let mut f = BufWriter::new(File::create(filename)?);

        let structs_with_funcs = get_structs_with_functions(api_defs);

        for api_def in api_defs {
            let base_filename = &api_def.base_filename;

            if api_def
                .structs
                .iter()
                .any(|s| !s.functions.is_empty() && !s.has_attribute("NoContext"))
            {
                writeln!(f, "#include \"{}.h\"", base_filename)?;
            }
        }

        writeln!(f)?;
        writeln!(f, "struct FlInternalData;")?;
        writeln!(f, "typedef void (*FlMainLoopCallback)(void* user_data);")?;
        writeln!(f, "\ntypedef struct FlApplication {{")?;
        writeln!(f, "   struct FlInternalData* priv;")?;
        writeln!(
            f,
            "    void (*main_loop)(FlMainLoopCallback callback, void* user_data);"
        )?;

        // Generate accesors for the various APIs
        for s in &structs_with_funcs {
            if let Some(func) = s.functions.iter().find(|f| f.name == s.name) {
                let fa = Self::generate_function_args(func, &s.name);
                writeln!(
                    f,
                    "    struct {}{}Api* (*{}_get_api)(struct FlInternalData* data, int api_version, {});",
                    C_API_SUFFIX,
                    s.name,
                    s.name.to_snake_case(),
                    arg_line(&fa.func_args, Ctx::No)
                )?;
            } else {
                writeln!(
                    f,
                    "    struct {}{}Api* (*{}_get_api)(struct FlInternalData* data, int api_version);",
                    C_API_SUFFIX,
                    s.name,
                    s.name.to_snake_case()
                )?;
            }
        }

        writeln!(f, "}} FlApplication;\n")?;
        writeln!(f, "FlApplication* fl_application_create_impl(FlApplicationSettings* settings, int version);\n")?;
        writeln!(
            f,
            "FL_INLINE bool fl_application_create(FlApplicationSettings* settings) {{"
        )?;
        writeln!(
            f,
            "    FlApp* api = fl_application_create_impl(settings, 0);"
        )?;

        for s in &structs_with_funcs {
            let name_snake = s.name.to_snake_case();
            writeln!(
                f,
                "    g_flowi_{}_api = api->{}_get_api(api->priv, 0);",
                name_snake, name_snake
            )?;
        }

        writeln!(f, "    return api;")?;
        writeln!(f, "}}\n")?;

        Ok(())
    }

    fn render_tera<W: Write, S: Serialize + Clone>(
        f: &mut W,
        data: &S,
        templates: &[&str],
        tera: &Tera,
    ) -> io::Result<()> {
        let t = data.clone();
        let ser_data = serde_json::to_value(t).unwrap();
        let ctx = Context::from_value(ser_data).unwrap();
        for template in templates {
            let m = tera.render(template, &ctx).unwrap();
            writeln!(f, "{}", m)?;
        }

        Ok(())
    }

    fn get_tera_value<'a>(arg: &str, args: &'a HashMap<String, Value>) -> tera::Result<&'a Value> {
        match args.get(arg) {
            Some(val) => Ok(val),
            None => Err("var parameter not found".into()),
        }
    }

    fn get_tera_string<'a>(arg: &str, args: &'a HashMap<String, Value>) -> tera::Result<&'a String> {
        match args.get(arg) {
            Some(val) => match val {
                Value::String(s) => Ok(s),
                _ => Err("self parameter is not a string".into()),
            }
            None => return Err("self_name parameter not found".into()),
        }
    }

    pub fn get_type_struct(args: &HashMap<String, Value>) -> tera::Result<Value> {
        let var_type = Self::get_tera_value("var", args)?;
        let self_name = Self::get_tera_string("self", args)?;

        let var: Variable = serde_json::from_value(var_type.clone()).unwrap();
        let c_type = Self::get_variable(&var, self_name);

        // for arrays we generate a pointer and a size
        match var.array {
            None => Ok(Value::String(format!("{} {};", c_type, var.name))),
            Some(ArrayType::Unsized) => {
                Ok(Value::String(format!("{}* {};\nuint32_t {}_size;", c_type, var.name, var.name)))
            }

            Some(ArrayType::SizedArray(ref size)) => {
                Ok(Value::String(format!("{}[{}];", var.name, size)))
            }
        }
    }

    pub fn is_struct_not_empty(args: &HashMap<String, Value>) -> tera::Result<Value> {
        let var_type = Self::get_tera_value("var", args)?;
        let var: Struct = serde_json::from_value(var_type.clone()).unwrap();
        Ok(Value::Bool(!var.variables.is_empty()))
    }

    pub fn generate(path: &str, api_def: &ApiDef, tera: &Tera) -> io::Result<()> {
        let filename = format!("{}/{}.h", path, api_def.base_filename);
        let inl_filename = format!("{}/{}.inl", path, api_def.base_filename);

        println!("    Generating Core C header: {}", filename);
        println!("    Generating Core Inl header: {}", inl_filename);

        {
            let mut f = BufWriter::new(File::create(&filename)?);
            let mut fi = BufWriter::new(File::create(&inl_filename)?);

            Self::render_tera(
                &mut f,
                &api_def,
                &[
                    "c_header.tera",
                    "c_enum.tera",
                    //"c_handle_type.tera",
                    "c_struct.tera",
                ],
                tera,
            )?;

            // generate defintion
            for sdef in &api_def.structs {
                for func in &sdef.functions {
                    let with_ctx = Ctx::No;

                    if sdef.has_attribute("Handle") {
                        Self::generate_function_def(&mut f, func, &sdef.name, with_ctx)?;
                    } else {
                        Self::generate_function_def(
                            &mut f,
                            func,
                            &format!("{}*", sdef.name),
                            with_ctx,
                        )?;
                    }
                }
            }

            for sdef in &api_def.structs {
                //let context_name = format!("struct {}{}Api* api", C_API_SUFFIX, sdef.name);

                // if we have functions for this struct and dynamic output we need to generate the
                // dispatch table
                if !sdef.functions.is_empty() && !sdef.has_attribute("NoContext") {
                    writeln!(fi, "typedef struct {}{}Api {{", C_API_SUFFIX, sdef.name)?;
                    writeln!(fi, "    struct FlInternalData* priv;")?;

                    for func in &sdef.functions {
                        Self::generate_function_dynamic(
                            &mut fi,
                            func,
                            &sdef.name,
                            Ctx::Yes("struct FlInternalData* priv"),
                        )?;
                    }

                    writeln!(fi, "}} {}{}Api;\n", C_API_SUFFIX, sdef.name)?;

                    writeln!(
                        fi,
                        "extern Fl{}Api* g_flowi_{}_api;\n",
                        sdef.name, sdef.name.to_snake_case()
                    )?;

                    writeln!(fi, "#ifdef FLOWI_STATIC")?;

                    for func in &sdef.functions {
                        let with_ctx = Ctx::Yes("struct FlInternalData* priv");
                        let fa = Self::generate_function_args(func, &sdef.name);

                        let func_name = format!("{}_{}_{}", C_API_SUFIX_FUNCS, &sdef.name.to_snake_case(), func.name);

                        #[rustfmt::skip]
                        writeln!(fi, "{} {}_impl({});", fa.return_value, &func_name, arg_line(&fa.internal_args, with_ctx))?;
                    }

                    writeln!(fi, "#endif\n")?;
                }

                for func in &sdef.functions {
                    let with_ctx = Ctx::No;
                    Self::generate_function(&mut fi, func, &sdef.name, with_ctx)?;
                }
            }

            writeln!(f, "\n#include \"{}.inl\"", api_def.base_filename)?;

            Self::render_tera(&mut f, &api_def, &["c_footer.tera"], tera)?;
        }

        run_clang_format(&filename);
        run_clang_format(&inl_filename);

        Ok(())
    }
}
