typedef struct FlImageApi {
  struct FlInternalData *priv;
  FlImage (*create_from_file)(struct FlInternalData *priv, FlString filename);
  FlImage (*create_from_memory)(struct FlInternalData *priv, FlString name,
                                uint8_t *data, uint32_t data_size);
  FlImageInfo *(*get_info)(struct FlInternalData *priv, FlImage image);
} FlImageApi;

extern FlImageApi *g_flowi_image_api;

#ifdef FLOWI_STATIC
FlImage fl_image_create_from_file_impl(struct FlInternalData *priv,
                                       FlString filename);
FlImage fl_image_create_from_memory_impl(struct FlInternalData *priv,
                                         FlString name, uint8_t *data,
                                         uint32_t data_size);
FlImageInfo *fl_image_get_info_impl(struct FlInternalData *priv, FlImage image);
#endif

// Load image from file. Supported formats are:
// JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock
// IJG lib) PNG 1/2/4/8/16-bit-per-channel TGA BMP non-1bpp, non-RLE PSD
// (composited view only, no extra channels, 8/16 bit-per-channel) GIF HDR
// (radiance rgbE format) PIC (Softimage PIC) PNM (PPM and PGM binary only)
FL_INLINE FlImage fl_image_create_from_file(const char *filename) {
  FlString filename_ = fl_cstr_to_flstring(filename);
#ifdef FLOWI_STATIC
  return fl_image_create_from_file_impl(g_flowi_image_api->priv, filename_);
#else
  return (g_flowi_image_api->create_from_file)(g_flowi_image_api->priv,
                                               filename_);
#endif
}

// Load image from memory. Supported formats are:
// JPEG baseline & progressive (12 bpc/arithmetic not supported, same as stock
// IJG lib) PNG 1/2/4/8/16-bit-per-channel TGA BMP non-1bpp, non-RLE PSD
// (composited view only, no extra channels, 8/16 bit-per-channel) GIF HDR
// (radiance rgbE format) PIC (Softimage PIC) PNM (PPM and PGM binary only)
FL_INLINE FlImage fl_image_create_from_memory(const char *name, uint8_t *data,
                                              uint32_t data_size) {
  FlString name_ = fl_cstr_to_flstring(name);
#ifdef FLOWI_STATIC
  return fl_image_create_from_memory_impl(g_flowi_image_api->priv, name_, data,
                                          data_size);
#else
  return (g_flowi_image_api->create_from_memory)(g_flowi_image_api->priv, name_,
                                                 data, data_size);
#endif
}

// Load SVG from file
// Load SVG from memory
// Get data amout the image
FL_INLINE FlImageInfo *fl_image_get_info(FlImage image) {
#ifdef FLOWI_STATIC
  return fl_image_get_info_impl(g_flowi_image_api->priv, image);
#else
  return (g_flowi_image_api->get_info)(g_flowi_image_api->priv, image);
#endif
}
