#include "qrcode.h"


typedef struct _qrcode_obj_t {
    mp_obj_base_t base;
	uint8_t *qr0;
	uint8_t *tempBuffer;
    enum qrcodegen_Ecc ecc;
    enum qrcodegen_Mask mask;
} qrcode_obj_t;

mp_obj_t qrcode___del__(mp_obj_t self_in) {
    qrcode_obj_t *self = MP_OBJ_TO_PTR(self_in);
	(void)self;
    return mp_const_none;
}

mp_obj_t qrcode_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    qrcode_obj_t *self = m_new_obj_with_finaliser(qrcode_obj_t);

    enum { ARG_ecc, ARG_mask };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_ecc, MP_ARG_INT, {.u_int = qrcodegen_Ecc_MEDIUM} },
        { MP_QSTR_mask, MP_ARG_INT, {.u_int = qrcodegen_Mask_AUTO} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    self->base.type = &qrcode_type;
    self->qr0 = m_new(uint8_t, qrcodegen_BUFFER_LEN_MAX);
	self->tempBuffer = m_new(uint8_t, qrcodegen_BUFFER_LEN_MAX);
    self->ecc = args[ARG_ecc].u_int;
    self->mask = args[ARG_mask].u_int;

    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t qrcode_set_text(mp_obj_t self_in, mp_obj_t text) {
    qrcode_obj_t *self = MP_OBJ_TO_PTR(self_in);

    if(mp_obj_is_str_or_bytes(text)) {
		// Defines str in-macro and populates it with the string contents
		// null-terminated
		GET_STR_DATA_LEN(text, str, str_len);

		bool ok = qrcodegen_encodeText((const char*)str,
			self->tempBuffer, self->qr0, self->ecc,
			qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
			self->mask, true);

		return ok ? mp_const_true : mp_const_false;
    }

	return mp_const_false;
}

mp_obj_t qrcode_get_module(mp_obj_t self_in, mp_obj_t x, mp_obj_t y) {
    qrcode_obj_t *self = MP_OBJ_TO_PTR(self_in);

	return qrcodegen_getModule(self->qr0, mp_obj_get_int(x), mp_obj_get_int(y)) ? mp_const_true : mp_const_false;
}

mp_obj_t qrcode_get_size(mp_obj_t self_in) {
    qrcode_obj_t *self = MP_OBJ_TO_PTR(self_in);

	mp_obj_t size = mp_obj_new_int(qrcodegen_getSize(self->qr0));

	mp_obj_t tuple[2];
    tuple[0] = size;
    tuple[1] = size;
    return mp_obj_new_tuple(2, tuple);
}

STATIC mp_int_t qrcode_get_buffer(mp_obj_t self_in, mp_buffer_info_t *bufinfo, mp_uint_t flags) {
    (void)flags;
    qrcode_obj_t *self = MP_OBJ_TO_PTR(self_in);
    bufinfo->buf = self->qr0;
    bufinfo->len = qrcodegen_BUFFER_LEN_MAX;
    bufinfo->typecode = 'B'; // view qr0 buffer as bytes
    return 0;
}

MP_DEFINE_CONST_FUN_OBJ_1(qrcode___del___obj, qrcode___del__);
MP_DEFINE_CONST_FUN_OBJ_2(qrcode_set_text_obj, qrcode_set_text);

MP_DEFINE_CONST_FUN_OBJ_1(qrcode_get_size_obj, qrcode_get_size);
MP_DEFINE_CONST_FUN_OBJ_3(qrcode_get_module_obj, qrcode_get_module);


STATIC const mp_rom_map_elem_t qrcode_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&qrcode___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_set_text), MP_ROM_PTR(&qrcode_set_text_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_size), MP_ROM_PTR(&qrcode_get_size_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_module), MP_ROM_PTR(&qrcode_get_module_obj) },
};

STATIC MP_DEFINE_CONST_DICT(qrcode_locals_dict, qrcode_locals_dict_table);

#ifdef MP_DEFINE_CONST_OBJ_TYPE
MP_DEFINE_CONST_OBJ_TYPE(
    qrcode_type,
    MP_QSTR_qrcode,
    MP_TYPE_FLAG_NONE,
    make_new, qrcode_make_new,
    buffer, qrcode_get_buffer,
    locals_dict, (mp_obj_dict_t*)&qrcode_locals_dict
);
#else
const mp_obj_type_t qrcode_type = {
    { &mp_type_type },
    .name = MP_QSTR_qrcode,
    .make_new = qrcode_make_new,
    .buffer_p = { .get_buffer = qrcode_get_buffer },
    .locals_dict = (mp_obj_dict_t*)&qrcode_locals_dict,
};
#endif

STATIC const mp_map_elem_t qrcode_globals_table[] = {
	{ MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_qrcode) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_QRCode), (mp_obj_t)&qrcode_type },

    { MP_ROM_QSTR(MP_QSTR_ECC_LOW), MP_ROM_INT(qrcodegen_Ecc_LOW) },
    { MP_ROM_QSTR(MP_QSTR_ECC_MEDIUM), MP_ROM_INT(qrcodegen_Ecc_MEDIUM) },
    { MP_ROM_QSTR(MP_QSTR_ECC_QUARTILE), MP_ROM_INT(qrcodegen_Ecc_QUARTILE) },
    { MP_ROM_QSTR(MP_QSTR_ECC_HIGH), MP_ROM_INT(qrcodegen_Ecc_HIGH) },

    { MP_ROM_QSTR(MP_QSTR_MASK_AUTO), MP_ROM_INT(qrcodegen_Mask_AUTO) },
    { MP_ROM_QSTR(MP_QSTR_MASK_0), MP_ROM_INT(qrcodegen_Mask_0) },
    { MP_ROM_QSTR(MP_QSTR_MASK_1), MP_ROM_INT(qrcodegen_Mask_1) },
    { MP_ROM_QSTR(MP_QSTR_MASK_2), MP_ROM_INT(qrcodegen_Mask_2) },
    { MP_ROM_QSTR(MP_QSTR_MASK_3), MP_ROM_INT(qrcodegen_Mask_3) },
    { MP_ROM_QSTR(MP_QSTR_MASK_4), MP_ROM_INT(qrcodegen_Mask_4) },
    { MP_ROM_QSTR(MP_QSTR_MASK_5), MP_ROM_INT(qrcodegen_Mask_5) },
    { MP_ROM_QSTR(MP_QSTR_MASK_6), MP_ROM_INT(qrcodegen_Mask_6) },
    { MP_ROM_QSTR(MP_QSTR_MASK_7), MP_ROM_INT(qrcodegen_Mask_7) },

    { MP_ROM_QSTR(MP_QSTR_MODE_NUMERIC), MP_ROM_INT(qrcodegen_Mode_NUMERIC) },
    { MP_ROM_QSTR(MP_QSTR_MODE_ALPHANUMERIC), MP_ROM_INT(qrcodegen_Mode_ALPHANUMERIC) },
    { MP_ROM_QSTR(MP_QSTR_MODE_BYTE), MP_ROM_INT(qrcodegen_Mode_BYTE) },
    { MP_ROM_QSTR(MP_QSTR_MODE_KANJI), MP_ROM_INT(qrcodegen_Mode_KANJI) },
    { MP_ROM_QSTR(MP_QSTR_MODE_ECI), MP_ROM_INT(qrcodegen_Mode_ECI) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_qrcode_globals, qrcode_globals_table);

const mp_obj_module_t qrcode_user_cmodule = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&mp_module_qrcode_globals,
};

#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_qrcode, qrcode_user_cmodule, MODULE_QRCODE_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_qrcode, qrcode_user_cmodule);
#endif
