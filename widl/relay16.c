#include "config.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string.h>
#include <ctype.h>

#include "widl.h"
#include "utils.h"
#include "parser.h"
#include "expr.h"
#include "typetree.h"
#include "typelib.h"
#include "header.h"

static void write_type_v(FILE *h, type_t *t, int is_field, int declonly, const char *name);

static void get_type_size16(const type_t *type, int *size, int *is_ptr);
static void write_args1632(FILE *h, const var_list_t *args, const char *name, int method)
{
    const var_t *arg;
    int count = 0;

    if (method == 1) {
        fprintf(h, "%s* This", name);
        count++;
    }
    if (args) LIST_FOR_EACH_ENTRY(arg, args, const var_t, entry) {
        int size, is_ptr;
        if (count) {
            fprintf(h, ", ");
        }
        get_type_size16(arg->type, &size, &is_ptr);
        if (size == 4)
        {
            if (is_ptr)
            {
                fprintf(h, "SEGPTR args16_%s", arg->name);
            }
            else
            {
                fprintf(h, "DWORD args16_%s", arg->name);
            }
        }
        else if (size == 2)
        {
            assert(!is_ptr);
            fprintf(h, "WORD args16_%s", arg->name);
        }
        else if (size == 8)
        {
            assert(!is_ptr);
            /* hard-coded */
            if (!strcmp(arg->type->c_name, "LARGE_INTEGER"))
            {
                fprintf(h, "LARGE_INTEGER args16_%s", arg->name);
            }
            else if (!strcmp(arg->type->c_name, "ULARGE_INTEGER"))
            {
                fprintf(h, "ULARGE_INTEGER args16_%s", arg->name);
            }
            else if (!strcmp(arg->type->c_name, "POINTL"))
            {
                fprintf(h, "POINTL args16_%s", arg->name);
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            assert(0);
        }
        count++;
    }
}

static void write_type_conv_macro(FILE *h, type_t *t)
{
    enum type_type type = type_get_type(t);
    if (type_is_alias(t))
    {
        fprintf(h, "%s", t->name);
        return;
    }
    switch (type)
    {
    case TYPE_VOID:
    {
        fprintf(h, "VOID");
        return;
    }
    case TYPE_BASIC:
        switch (type_basic_get_type(t))
        {
        case TYPE_BASIC_INT8:
            fprintf(h, "INT8");
            return;
        case TYPE_BASIC_INT16:
            fprintf(h, "INT16");
            return;
        case TYPE_BASIC_INT32:
            fprintf(h, "INT32");
            return;
        case TYPE_BASIC_INT64:
            fprintf(h, "INT64");
            return;
        case TYPE_BASIC_INT:
            fprintf(h, "INT");
            return;
        case TYPE_BASIC_INT3264:
            fprintf(h, "INT3264");
            return;
        case TYPE_BASIC_CHAR:
            fprintf(h, "CHAR");
            return;
        case TYPE_BASIC_HYPER:
            fprintf(h, "HYPER");
            return;
        case TYPE_BASIC_BYTE:
            fprintf(h, "BYTE");
            return;
        case TYPE_BASIC_WCHAR:
            fprintf(h, "WCHAR");
            return;
        case TYPE_BASIC_ERROR_STATUS_T:
        case TYPE_BASIC_FLOAT:
            assert((TYPE_BASIC_FLOAT, 0));
        case TYPE_BASIC_DOUBLE:
            assert((TYPE_BASIC_DOUBLE, 0));
        case TYPE_BASIC_HANDLE:
            assert((TYPE_BASIC_HANDLE, 0));
        }
        break;
    case TYPE_ENUM:
        break;
    case TYPE_STRUCT:
        assert(type_get_name(t, NAME_DEFAULT));
        fprintf(h, "STRUCT_%s", type_get_name(t, NAME_DEFAULT));
        return;
    case TYPE_ENCAPSULATED_UNION:
        break;
    case TYPE_UNION:
        break;
    case TYPE_ALIAS:
        break;
    case TYPE_MODULE:
        break;
    case TYPE_COCLASS:
        break;
    case TYPE_FUNCTION:
        break;
    case TYPE_INTERFACE:
    {
        fprintf(h, "IFACE_%s", t->name);
        return;
    }
    case TYPE_POINTER:
    {
        fprintf(h, "PTR_");
        write_type_conv_macro(h, type_pointer_get_ref(t));
        return;
    }
    case TYPE_ARRAY:
        if (t->name && type_array_is_decl_as_ptr(t))
            fprintf(h, "%s", t->name);
        else
        {
            write_type_conv_macro(h, type_array_get_element(t));
            if (type_array_is_decl_as_ptr(t))
                fprintf(h, "_PTR");
        }
        return;
    case TYPE_BITFIELD:
        break;
    default:
        break;
    }
    assert(0);
}

static void write_type_conv3216(FILE *h, type_t *t, const char *expr_prefix, const char *expr, const char *to, const char *to2, int unmap)
{
    enum type_type typtyp1 = type_get_type(t);
    if (typtyp1 == TYPE_BASIC)
    {
        switch (type_basic_get_type(t))
        {
        case TYPE_BASIC_INT8:
        case TYPE_BASIC_INT16:
        case TYPE_BASIC_INT32:
        case TYPE_BASIC_INT64:
            if (unmap)
                return;
            fprintf(h, "    %s%s = %s%s;\n", to, to2, expr_prefix, expr);
            return;
        }
    }
    type_t *typ2 = typtyp1 == TYPE_POINTER ? type_pointer_get_ref(t) : NULL;
    enum type_type typtyp2 = typ2 ? type_get_type(typ2) : -1;
    if (typtyp2 == TYPE_INTERFACE)
    {
        if (unmap)
            return;
        fprintf(h, "    %s%s = iface32_16(&IID_%s, %s%s);\n", to, to2, typ2->name, expr_prefix, expr);
        return;
    }
    if (unmap)
        fprintf(h, "    UNMAP_");
    else
        fprintf(h, "    MAP_");
    write_type_conv_macro(h, t);
    fprintf(h, "32_16(%s%s, %s%s)", to, to2, expr_prefix, expr);
    fprintf(h, ";\n");
}
static void write_type_conv1632(FILE *h, type_t *t, const char *expr_prefix, const char *expr, const char *to, const char *to2, int unmap)
{
    enum type_type typtyp1 = type_get_type(t);
    type_t *typ2 = typtyp1 == TYPE_POINTER ? type_pointer_get_ref(t) : NULL;
    enum type_type typtyp2 = typ2 ? type_get_type(typ2) : -1;
    if (typtyp1 == TYPE_BASIC)
    {
        switch (type_basic_get_type(t))
        {
        case TYPE_BASIC_INT8:
        case TYPE_BASIC_INT16:
        case TYPE_BASIC_INT32:
        case TYPE_BASIC_INT64:
            if (unmap)
                return;
            fprintf(h, "    %s%s = %s%s;\n", to, to2, expr_prefix, expr);
            return;
        }
    }
    if (typtyp2 == TYPE_INTERFACE)
    {
        if (unmap)
            return;
        fprintf(h, "    %s%s = iface16_32(&IID_%s, %s%s);\n", to, to2, typ2->name, expr_prefix, expr);
        return;
    }
    if (unmap)
        fprintf(h,"    UNMAP_");
    else
        fprintf(h, "    MAP_");
    write_type_conv_macro(h, t);
    fprintf(h, "16_32(%s%s, %s%s)", to, to2, expr_prefix, expr);
    fprintf(h, ";\n");

    if (0)
    {
        enum type_type typtyp1 = type_get_type(t);
        type_t *typ2 = typtyp1 == TYPE_POINTER ? type_pointer_get_ref(t) : NULL;
        enum type_type typtyp2 = typ2 ? type_get_type(typ2) : -1;
        type_t *typ3 = typ2 && typtyp2 == TYPE_POINTER ? type_pointer_get_ref(typ2) : NULL;
        enum type_type typtyp3 = typ3 ? type_get_type(typ3) : -1;
        if (typtyp1 == TYPE_POINTER)
        {
            /* pointer conversion */
            fprintf(h, "(");
            write_type_decl_left(h, t);
            fprintf(h, ")MapSL(%s%s)", expr_prefix, expr);
        }
        else
        {
            /* cast conversion */
            fprintf(h, "(");
            write_type_decl_left(h, t);
            fprintf(h, ")%s%s", expr_prefix, expr);
        }
    }
}

static const var_t *find_args(const var_list_t *args, const char *name)
{
    const var_t *arg;

    if (args) LIST_FOR_EACH_ENTRY(arg, args, const var_t, entry) {
        if (!strcmp(arg->name, name))
            return arg;
    }
    return NULL;
}
/* hard-coded */
static const char *handle16_table[] =
{
    "HTASK",
    "HWND",
    "HMENU",
    "HICON",
    "HBITMAP",
    "HOLEMENU",
    "HDC",
    "HGLOBAL",
};
static void get_type_size16(const type_t *type, int *size, int *is_ptr)
{
    enum type_type typtyp1 = type_get_type(type);
    *size = 0;
    *is_ptr = 0;
    if (type_is_alias(type))
    {
        if (*type->name == 'H')
        {
            int i;
            for (i = 0; i < sizeof(handle16_table) / sizeof(*handle16_table); i++)
            {
                if (!strcmp(type->name, handle16_table[i]))
                {
                    *size = 2;
                    return;
                }
            }
            if (strcmp(type->name, "HRESULT"))
            {
                *size = 0;
            }
        }
        if (!strcmp(type->name, "BOOL"))
        {
            *size = 2;
            return;
        }
    }
    switch (typtyp1)
    {
    case TYPE_ENUM:
        /* fixme:ATTR_V1ENUM */
        *size = 2;
        break;
    case TYPE_INTERFACE:
    case TYPE_POINTER:
        *size = 4;
        *is_ptr = 1;
        break;
    case TYPE_BASIC:
        switch (type_basic_get_type(type))
        {
        case TYPE_BASIC_INT8:
            *size = 1;
            break;
        case TYPE_BASIC_INT16:
            *size = 2;
            break;
        case TYPE_BASIC_INT32:
            *size = 4;
            break;
        case TYPE_BASIC_INT64:
            *size = 8;
            break;
        case TYPE_BASIC_INT:
            *size = 4;
            break;
        case TYPE_BASIC_INT3264:
            *size = 4;
            break;
        case TYPE_BASIC_CHAR:
            *size = 1;
            break;
        case TYPE_BASIC_HYPER:
            assert((TYPE_BASIC_HYPER, 0));
        case TYPE_BASIC_BYTE:
            *size = 1;
            break;
        case TYPE_BASIC_WCHAR:
            *size = 2;
            break;
        case TYPE_BASIC_ERROR_STATUS_T:
        case TYPE_BASIC_FLOAT:
            assert((TYPE_BASIC_FLOAT, 0));
        case TYPE_BASIC_DOUBLE:
            assert((TYPE_BASIC_DOUBLE, 0));
        case TYPE_BASIC_HANDLE:
            assert((TYPE_BASIC_HANDLE, 0));
        }
        break;
    case TYPE_ARRAY:
        *size = 4;
        *is_ptr = 1;
        break;
    case TYPE_STRUCT:
        /* hard-coded */
        if (!strcmp("LARGE_INTEGER", type_get_name(type, NAME_DEFAULT)))
        {
            *size = 8;
            break;
        }
        if (!strcmp("ULARGE_INTEGER", type_get_name(type, NAME_DEFAULT)))
        {
            *size = 8;
            break;
        }
        if (!strcmp("SIZE", type_get_name(type, NAME_DEFAULT)))
        {
            *size = 4;
            break;
        }
        if (!strcmp("POINTL", type_get_name(type, NAME_DEFAULT)))
        {
            *size = 8;
            break;
        }
        /* FIXME */
        if (!strcmp("IViewObjectCallback", type_get_name(type, NAME_DEFAULT)))
        {
            *size = 4;
            break;
        }
        assert(0);
    case TYPE_VOID:
    case TYPE_ENCAPSULATED_UNION:
    case TYPE_UNION:
    case TYPE_ALIAS:
    case TYPE_MODULE:
    case TYPE_COCLASS:
    case TYPE_FUNCTION:
    case TYPE_BITFIELD:
    default:
        assert(0);
    }
    return;
}

enum args_conv
{
    ARGS_CONV_DECL,
    ARGS_CONV_AFTER_CONV,
    ARGS_CONV_CALL,
    ARGS_CONV_MAP,
    ARGS_CONV_UNMAP,
    ARGS_CONV_TRACE,
    ARGS_CONV_TRACE_ARGS,
};
static void write_type_decl_left3216(FILE *f, type_t *t);
static void write_args_conv(FILE *h, const var_list_t *args, const char *name, enum args_conv args_conv, int is_1632)
{
    const var_t *arg;
    int count = 0;
    int ivar_defined = FALSE;
    if (args_conv == ARGS_CONV_TRACE)
    {
        if (is_1632)
            fprintf(h, "%s", "%04x:%04x(%p)");
        else
            fprintf(h, "%s", "%p(%04x:%04x)");
    }
    if (args_conv == ARGS_CONV_TRACE_ARGS)
    {
        if (is_1632)
            fprintf(h, ", SELECTOROF(This), OFFSETOF(This), iface32");
        else
            fprintf(h, ", This, SELECTOROF(iface16), OFFSETOF(iface16)");
    }
    if (args) LIST_FOR_EACH_ENTRY(arg, args, const var_t, entry) {
        /* ATTR_IIDIS ATTR_OUT ATTR_IN */
        expr_t *iid = get_attrp(arg->attrs, ATTR_IIDIS);
        int is_out = is_attr(arg->attrs, ATTR_OUT);
        int is_in = is_attr(arg->attrs, ATTR_IN);
        int is_array = 0;
        enum type_type typtyp1 = type_get_type(arg->type);
        type_t *typ2 = typtyp1 == TYPE_POINTER ? type_pointer_get_ref(arg->type) : NULL;
        enum type_type typtyp2 = typ2 ? type_get_type(typ2) : -1;
        type_t *typ3 = typ2 && typtyp2 == TYPE_POINTER ? type_pointer_get_ref(typ2) : NULL;
        enum type_type typtyp3 = typ3 ? type_get_type(typ3) : -1;
        if (typtyp1 == TYPE_ARRAY)
        {
            type_t *array_elem_type = type_array_get_element(arg->type);
            enum type_type array_elem_type_type = type_get_type(array_elem_type);
            if (array_elem_type_type != TYPE_VOID)
            {
                typ2 = array_elem_type;
                typtyp1 = array_elem_type_type;
                is_array = TRUE;
            }
            else
            {
                is_out = 0;
            }
        }
        if (args_conv == ARGS_CONV_CALL)
        {
            if (!is_in && is_out)
            {
                if (is_1632)
                {
                    if (is_array)
                    {
                        fprintf(h, ", args32_%s", arg->name);
                    }
                    else
                    {
                        fprintf(h, ", &args32_%s", arg->name);
                    }
                }
                else
                {
                    if (is_array)
                    {
                        fprintf(h, ", MapLS(args16_%s)", arg->name);
                    }
                    else
                    {
                        fprintf(h, ", MapLS(&args16_%s)", arg->name);
                    }
                }
            }
            else
            {
                fprintf(h, ", args%d_%s", is_1632 ? 32 : 16, arg->name);
            }
        }
        else if (args_conv == ARGS_CONV_MAP || args_conv == ARGS_CONV_UNMAP)
        {
            type_t *map_type;
            const char *prefix_src;
            const char *prefix_dst;
            map_type = arg->type;
            if (is_1632)
                prefix_dst = "args32_";
            else
                prefix_dst = "args16_";
            if (is_1632)
                prefix_src = "args16_";
            else
                prefix_src = "";

            if (is_in)
            {
                if (is_1632)
                {
                    write_type_conv1632(h, map_type, prefix_src, arg->name, prefix_dst, arg->name, args_conv == ARGS_CONV_UNMAP);
                }
                else
                {
                    write_type_conv3216(h, map_type, prefix_src, arg->name, prefix_dst, arg->name, args_conv == ARGS_CONV_UNMAP);
                }
            }
        }
        else if (args_conv == ARGS_CONV_DECL)
        {
            if (is_out && !is_in)
            {
                assert(typ2);
                if (is_array && !ivar_defined)
                {
                    fprintf(h, "    int i__;\n");
                    if (is_1632)
                        fprintf(h, "    void *dst__;\n");
                    ivar_defined = 1;
                }
                /* indent */
                fprintf(h, "    ");
                if (is_1632)
                    write_type_decl_left(h, typ2);
                else
                    write_type_decl_left3216(h, typ2);
                /* [out] interface conversion */
                if (is_array)
                {
                    int count = 0;
                    expr_t *e;
                    expr_list_t *size_is = get_attrp(arg->attrs, ATTR_SIZEIS);
                    assert(size_is);
                    fprintf(h, " *args%d_%s = ", is_1632 ? 32 : 16, arg->name);
                    fprintf(h, " IFACE_ALLOC_ARRAY(");
                    if (is_1632)
                        write_type_decl_left(h, typ2);
                    else
                        write_type_decl_left3216(h, typ2);
                    fprintf(h, " ,");
                    LIST_FOR_EACH_ENTRY(e, size_is, expr_t, entry)
                    {
                        write_expr(h, e, 1, 1, NULL, NULL, is_1632 ? "args16_" : "");
                        assert(count++ == 0);
                    }
                    fprintf(h, " );\n");

                }
                else
                {
                    fprintf(h, " args%d_%s = {0};\n", is_1632 ? 32 : 16, arg->name);
                }
            }
            else
            {
                fprintf(h, "    ");
                if (is_1632)
                    write_type_decl_left(h, arg->type);
                else
                    write_type_decl_left3216(h, arg->type);
                fprintf(h, " args%d_%s;\n", is_1632 ? 32 : 16, arg->name);
            }
        }
        else if (args_conv == ARGS_CONV_AFTER_CONV)
        {
            /* [out] conversion */
            if (iid && is_out)
            {
                /* indent */
                assert(iid->type == EXPR_IDENTIFIER);
                const char *iid_arg_name = iid->u.sval;
                const var_t *iid_arg = find_args(args, iid_arg_name);
                assert(iid_arg);
                fprintf(h, "    ");
                /* args32_riid? args16_riid? */
                if (is_1632)
                    fprintf(h, "*(SEGPTR*)MapSL(args16_%s) = iface32_16(args32_%s, args32_%s);\n", arg->name, iid_arg_name, arg->name);
                else
                    fprintf(h, "*%s = iface16_32(%s, args16_%s);\n", arg->name, iid_arg_name, arg->name);
            }
            else if (typ2 && is_out)
            {
                char buf[256];
                char buf2[256];
                if (is_array)
                {
                    int count = 0;
                    expr_t *e;
                    expr_list_t *len_is = get_attrp(arg->attrs, ATTR_LENGTHIS);
                    assert(len_is);
                    if (is_1632)
                    {
                        fprintf(h, "    dst__ = MapSL(args16_%s);\n", arg->name);
                    }
                    fprintf(h, "    for (i__ = 0; i__ < (");
                    LIST_FOR_EACH_ENTRY(e, len_is, expr_t, entry)
                    {
                        write_expr(h, e, 1, 1, NULL, NULL, is_1632 ? "&args32_" : "&args16_");
                        assert(count++ == 0);
                    }
                    fprintf(h, ") ; i__++)\n");
                    fprintf(h, "    {\n");
                    fprintf(h, "    ");
                    if (is_1632)
                    {
                        snprintf(buf2, 256, "args32_%s[i__]", arg->name); /* FIXME */
                        write_type_conv3216(h, typ2, buf2, "", "((SEGPTR*)dst__)[i__]", "", FALSE);
                    }
                    else
                    {
                        snprintf(buf2, 256, "args16_%s[i__]", arg->name); /* FIXME */
                        write_type_conv1632(h, typ2, buf2, "", arg->name, "[i__]", FALSE);
                    }
                    fprintf(h, "    }\n");
                    if (is_1632)
                        fprintf(h, "    IFACE_FREE_ARRAY(args16_%s);\n", arg->name);
                    else
                        fprintf(h, "    IFACE_FREE_ARRAY(args32_%s);\n", arg->name);
                }
                else
                {
                    if (is_1632)
                    {
                        fprintf(h, "    if (args16_%s)\n    {\n    ", arg->name);
                        snprintf(buf, 256, "*(SEGPTR*)MapSL(args16_%s)", arg->name); /* FIXME */
                        write_type_conv3216(h, is_in ? arg->type : typ2, "args32_", arg->name, buf, "", FALSE);
                        fprintf(h, "    }\n", arg->name);
                    }
                    else
                    {
                        fprintf(h, "    if (%s)\n    {\n    ", arg->name);
                        write_type_conv1632(h, is_in ? arg->type : typ2, "args16_", arg->name, is_in ? "" : "*", arg->name, FALSE);
                        fprintf(h, "    }\n", arg->name);
                    }
                }
            }
        }
        else if (args_conv == ARGS_CONV_TRACE)
        {
            int size, is_ptr;
            get_type_size16(arg->type, &size, &is_ptr);
            if (is_1632 && is_ptr)
            {
                fprintf(h, ",%s", "%08x");
            }
            else
            {
                if (is_ptr)
                {
                    fprintf(h, ",%s", "%p");
                }
                else if (size == 8)
                {
                    fprintf(h, ",%s", "%016llx");
                }
                else
                {
                    fprintf(h, ",%s", "%08x");
                }
            }
        }
        else if (args_conv == ARGS_CONV_TRACE_ARGS)
        {
            fprintf(h, ", %s%s", is_1632 ? "args16_" : "", arg->name);
        }
        else
        {
            assert(0);
        }
        count++;
    }
}

static int indentation;
static void write_type_left3216(FILE *h, type_t *t, enum name_type name_type, int declonly, const char *typ_prefix)
{
  const char *name;

  if (!h) return;

  name = type_get_name(t, name_type);

  if (is_attr(t->attrs, ATTR_CONST) &&
      (type_is_alias(t) || !is_ptr(t)))
    fprintf(h, "const ");

  if (type_is_alias(t)) fprintf(h, "%s%s", typ_prefix, t->name);
  else {
    switch (type_get_type_detect_alias(t)) {
      case TYPE_ENUM:
        if (!declonly && t->defined && !t->written) {
          assert(0);
        }
        else fprintf(h, "enum %s%s", t->name ? typ_prefix : "", name ? name : "");
        break;
      case TYPE_STRUCT:
      case TYPE_ENCAPSULATED_UNION:
        if (!declonly && t->defined && !t->written) {
          assert(0);
        }
        else fprintf(h, "struct %s%s", t->name ? typ_prefix : "", name ? name : "");
        break;
      case TYPE_UNION:
        if (!declonly && t->defined && !t->written) {
          assert(0);
        }
        else fprintf(h, "union %s%s", t->name ? typ_prefix : "", t->name ? t->name : "");
        break;
      case TYPE_POINTER:
        fprintf(h, "SEGPTR");

        //write_type_left3216(h, type_pointer_get_ref(t), name_type, declonly, typ_prefix);
        ////fprintf(h, "%s*", needs_space_after(type_pointer_get_ref(t)) ? " " : "");
        //if (is_attr(t->attrs, ATTR_CONST)) fprintf(h, "const ");
        break;
      case TYPE_ARRAY:
        fprintf(h, "SEGPTR");
        //if (t->name && type_array_is_decl_as_ptr(t))
        //  fprintf(h, "%s%s", typ_prefix, t->name);
        //else
        //{
        //  write_type_left3216(h, type_array_get_element(t), name_type, declonly, typ_prefix);
        //  if (type_array_is_decl_as_ptr(t))
        //    fprintf(h, "%s", needs_space_after(type_array_get_element(t)) ? " " : "");
        //}
        break;
      case TYPE_BASIC:
        if (type_basic_get_type(t) != TYPE_BASIC_INT32 &&
            type_basic_get_type(t) != TYPE_BASIC_INT64 &&
            type_basic_get_type(t) != TYPE_BASIC_HYPER)
        {
          if (type_basic_get_sign(t) < 0) fprintf(h, "signed ");
          else if (type_basic_get_sign(t) > 0) fprintf(h, "unsigned ");
        }
        switch (type_basic_get_type(t))
        {
        case TYPE_BASIC_INT8: fprintf(h, "small"); break;
        case TYPE_BASIC_INT16: fprintf(h, "short"); break;
        case TYPE_BASIC_INT: fprintf(h, "int"); break;
        case TYPE_BASIC_INT3264: fprintf(h, "__int3264"); break;
        case TYPE_BASIC_BYTE: fprintf(h, "byte"); break;
        case TYPE_BASIC_CHAR: fprintf(h, "char"); break;
        case TYPE_BASIC_WCHAR: fprintf(h, "wchar_t"); break;
        case TYPE_BASIC_FLOAT: fprintf(h, "float"); break;
        case TYPE_BASIC_DOUBLE: fprintf(h, "double"); break;
        case TYPE_BASIC_ERROR_STATUS_T: fprintf(h, "error_status_t"); break;
        case TYPE_BASIC_HANDLE: fprintf(h, "handle_t"); break;
        case TYPE_BASIC_INT32:
          if (type_basic_get_sign(t) > 0)
            fprintf(h, "ULONG");
          else
            fprintf(h, "LONG");
          break;
        case TYPE_BASIC_INT64:
          if (type_basic_get_sign(t) > 0)
            fprintf(h, "UINT64");
          else
            fprintf(h, "INT64");
          break;
        case TYPE_BASIC_HYPER:
          if (type_basic_get_sign(t) > 0)
            fprintf(h, "MIDL_uhyper");
          else
            fprintf(h, "hyper");
          break;
        }
        break;
      case TYPE_INTERFACE:
      case TYPE_MODULE:
      case TYPE_COCLASS:
        fprintf(h, "%s%s", typ_prefix, t->name);
        break;
      case TYPE_VOID:
        fprintf(h, "void");
        break;
      case TYPE_BITFIELD:
        write_type_left3216(h, type_bitfield_get_field(t), name_type, declonly, typ_prefix);
        break;
      case TYPE_ALIAS:
      case TYPE_FUNCTION:
        /* handled elsewhere */
        assert(0);
        break;
    }
  }
}
static void write_type_decl_left3216(FILE *f, type_t *t)
{
  write_type_left3216(f, t, NAME_DEFAULT, TRUE, "TYP16_");
}

static void write_32_16_func(FILE *header, const statement_t *stmt, const type_t *iface, const char *name)
{
    const var_t *func = stmt->u.var;
    if (!is_callas(func->attrs)) {
        const char *callconv = get_attrp(func->type->attrs, ATTR_CALLCONV);
        if (!callconv) callconv = "STDMETHODCALLTYPE";
        write_type_decl_left(header, type_function_get_rettype(func->type));
        fprintf(header, " %s %s_32_16_%s(%s *This", callconv, name, get_name(func), name);
        if (type_get_function_args(func->type)) {
            fprintf(header, ", ");
            write_args(header, type_get_function_args(func->type), name, 0, 0);
        }
        fprintf(header, ")\n");
        fprintf(header, "{\n");
        fprintf(header, "    SEGPTR iface16 = get_interface16(This);\n");
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_DECL, FALSE);
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_MAP, FALSE);
        fprintf(header, "    TRACE(\"(");
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_TRACE, FALSE);
        fprintf(header, ")\\n\"");
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_TRACE_ARGS, FALSE);
        fprintf(header, ");\n");
        fprintf(header, "    ");
        type_t *ret_type = type_function_get_rettype(func->type);
        if (type_get_type(ret_type) != TYPE_VOID)
        {
            write_type_decl_left(header, ret_type);
            fprintf(header, " result = (");
            write_type_decl_left(header, ret_type);
            fprintf(header, ")");
        }
        fprintf(header, "%s16_%s(iface16", name, get_name(func));
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_CALL, FALSE);
        fprintf(header, ");\n");
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_UNMAP, FALSE);
        write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_AFTER_CONV, FALSE);
        if (type_get_type(ret_type) != TYPE_VOID)
        {
            fprintf(header, "    return result;\n");
        }
        fprintf(header, "}\n");
    }
}
static void write_16_32_func(FILE *header, const statement_t *stmt, const type_t *iface, const char *name)
{
    const var_t *func = stmt->u.var;
    const char *callconv = get_attrp(func->type->attrs, ATTR_CALLCONV);
    if (!callconv) callconv = "STDMETHODCALLTYPE";
    write_type_decl_left(header, type_function_get_rettype(func->type));
    fprintf(header, " %s %s_16_32_%s(SEGPTR This", callconv = "CDECL", name, get_name(func));
    if (type_get_function_args(func->type)) {
        fprintf(header, ", ");
        write_args1632(header, type_get_function_args(func->type), name, 0);
    }
    fprintf(header, ")\n");
    fprintf(header, "{\n");
    fprintf(header, "    %s *iface32 = (%s*)get_interface32(This);\n", name, name);
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_DECL, TRUE);
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_MAP, TRUE);
    fprintf(header, "    TRACE(\"(");
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_TRACE, TRUE);
    fprintf(header, ")\\n\"");
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_TRACE_ARGS, TRUE);
    fprintf(header, ");\n");
    fprintf(header, "    ");
    type_t *ret_type = type_function_get_rettype(func->type);
    if (type_get_type(ret_type) != TYPE_VOID)
    {
        write_type_decl_left(header, type_function_get_rettype(func->type));
        fprintf(header, " result = (");
        write_type_decl_left(header, type_function_get_rettype(func->type));
        fprintf(header, ")");
    }
    fprintf(header, "iface32->lpVtbl->%s(iface32", get_name(func));
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_CALL, TRUE);
    fprintf(header, ");\n");
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_UNMAP, TRUE);
    write_args_conv(header, type_get_function_args(func->type), name, ARGS_CONV_AFTER_CONV, TRUE);
    if (type_get_type(ret_type) != TYPE_VOID)
    {
        fprintf(header, "    return result;\n");
    }
    fprintf(header, "}\n");
}
static void do_write_c_method_def(FILE *header, const type_t *iface, const char *name)
{
    const statement_t *stmt;
    int first_iface = 1;

    if (type_iface_get_inherit(iface))
        do_write_c_method_def(header, type_iface_get_inherit(iface), name);

    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
        const var_t *func = stmt->u.var;
        if (first_iface) {
            fprintf(header, "/*** %s methods ***/\n", iface->name);
            first_iface = 0;
        }
        if (!is_callas(func->attrs)) {
            write_16_32_func(header, stmt, iface, name);
            write_32_16_func(header, stmt, iface, name);
        }
    }
}

static int filter_iface(const char *name)
{
    return 1;
}
static void write_thunk_1632_3216(FILE *file, type_t *iface)
{
    if (filter_iface(iface->c_name))
    {
        /* fprintf(file, "typedef interface %s %s;\n", iface->c_name, iface->c_name); */
        do_write_c_method_def(file, iface, iface->c_name);
    }
}
static void write_guid(FILE *f, const UUID *uuid)
{
    if (!uuid) return;
    fprintf(f, "{0x%08x, 0x%04x, 0x%04x, {0x%02x,0x%02x, 0x%02x,"
        "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x}}",
        uuid->Data1, uuid->Data2, uuid->Data3, uuid->Data4[0],
        uuid->Data4[1], uuid->Data4[2], uuid->Data4[3], uuid->Data4[4], uuid->Data4[5],
        uuid->Data4[6], uuid->Data4[7]);
}

static void do_write_vtable32(FILE *header, const type_t *iface, const char *name)
{
    const statement_t *stmt;
    int first_iface = 1;

    if (type_iface_get_inherit(iface))
        do_write_vtable32(header, type_iface_get_inherit(iface), name);

    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
        const var_t *func = stmt->u.var;
        if (first_iface) {
            fprintf(header, "    /*** %s methods ***/\n", iface->name);
            first_iface = 0;
        }
        if (!is_callas(func->attrs)) {
            fprintf(header, "    %s_32_16_%s,\n", name, func->name);
        }
    }
}

static void write_args16_str(FILE *header, const var_list_t *args)
{
    const var_t *arg;

    fprintf(header, "s");
    if (args) LIST_FOR_EACH_ENTRY(arg, args, const var_t, entry) {
        int size, is_ptr;
        get_type_size16(arg->type, &size, &is_ptr);
        if (size == 2)
        {
            fprintf(header, "w");
        }
        if (size == 4)
        {
            if (is_ptr)
                fprintf(header, "s");
            else
                fprintf(header, "l");
        }
        if (size == 8)
        {
            fprintf(header, "ll");
        }
    }
    return NULL;
}
static void do_write_vtable16(FILE *header, const type_t *iface, const char *name)
{
    const statement_t *stmt;
    int first_iface = 1;

    if (type_iface_get_inherit(iface))
        do_write_vtable16(header, type_iface_get_inherit(iface), name);

    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
        const var_t *func = stmt->u.var;
        if (first_iface) {
            fprintf(header, "    /*** %s methods ***/\n", iface->name);
            first_iface = 0;
        }
        if (!is_callas(func->attrs)) {
            fprintf(header, "    {");
            fprintf(header, "%s_16_32_%s,\"%s::%s\",\"", name, func->name, name, func->name);
            write_args16_str(header, type_get_function_args(func->type));
            fprintf(header, "\"},\n");
        }
    }
}

static type_t **sort_interfaces(const statement_list_t *stmts, int *count);
static void write_vtable(FILE *file, const statement_list_t *stmts)
{
    const statement_t *stmt;
    if (stmts) LIST_FOR_EACH_ENTRY(stmt, stmts, const statement_t, entry)
    {
        switch (stmt->type)
        {
        case STMT_TYPE:
            if (type_get_type(stmt->u.type) == TYPE_INTERFACE)
            {
                if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
                {
                    if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
                    {
                        const type_t *type = stmt->u.type;
                        if (!filter_iface(type->c_name))
                            continue;
                        fprintf(file, "%sVtbl %s_vtbl =\n{\n", type->c_name, type->c_name);
                        do_write_vtable32(file, type, type->c_name);
                        fprintf(file, "};\n");
                        fprintf(file, "%s16Vtbl %s_vtbl16;\n", type->c_name, type->c_name);
                        fprintf(file, "vtbl_entry %s_vtbl16_entries[] =\n{\n", type->c_name);
                        do_write_vtable16(file, type, type->c_name);
                        fprintf(file, "    {0}\n};\n");
                    }
                }
            }
            break;
        }
    }
}

static int cmp_iid( const void *ptr1, const void *ptr2 )
{
    const type_t * const *iface1 = ptr1;
    const type_t * const *iface2 = ptr2;
    const UUID *uuid1 = get_attrp( (*iface1)->attrs, ATTR_UUID );
    const UUID *uuid2 = get_attrp( (*iface2)->attrs, ATTR_UUID );
    return memcmp( uuid1, uuid2, sizeof(UUID) );
}

static void build_iface_list( const statement_list_t *stmts, type_t **ifaces[], int *count )
{
    const statement_t *stmt;

    if (!stmts) return;
    LIST_FOR_EACH_ENTRY( stmt, stmts, const statement_t, entry )
    {
        if (stmt->type == STMT_TYPE && type_get_type(stmt->u.type) == TYPE_INTERFACE)
        {
            type_t *iface = stmt->u.type;
            *ifaces = xrealloc( *ifaces, (*count + 1) * sizeof(**ifaces) );
            (*ifaces)[(*count)++] = iface;
        }
    }
}

static type_t **sort_interfaces( const statement_list_t *stmts, int *count )
{
    type_t **ifaces = NULL;

    *count = 0;
    build_iface_list( stmts, &ifaces, count );
    qsort( ifaces, *count, sizeof(*ifaces), cmp_iid );
    return ifaces;
}

static void write_vtable2(FILE *file, const statement_list_t *stmts)
{
    const statement_t *stmt;
    int count;
    type_t **types;
    types = sort_interfaces(stmts, &count);
    fprintf(file, "interface_entry interfaces[] =\n{\n");
    for (int i = 0; i < count; i++)
    {
        if (type_get_type(types[i]) == TYPE_INTERFACE)
        {
            if (is_object(types[i]) || is_attr(types[i]->attrs, ATTR_DISPINTERFACE))
            {
                if (is_object(types[i]) || is_attr(types[i]->attrs, ATTR_DISPINTERFACE))
                {
                    const type_t *type = types[i];
                    if (!filter_iface(type->c_name))
                        continue;
                    const UUID *uuid = get_attrp(type->attrs, ATTR_UUID);
                    fprintf(file, "    {\n        ");
                    write_guid(file, uuid);
                    fprintf(file, ",\n        &%s_vtbl,\n", type->c_name);
                    fprintf(file, "        %s_vtbl16_entries,\n", type->c_name);
                    fprintf(file, "        &%s_vtbl16\n", type->c_name);
                    fprintf(file, "    },\n");
                }
            }
        }
    }
    fprintf(file, "};\n");
    fprintf(file, "size_t interfaces_count = %d;\n", count);
}
static void write_thunks_1632_3216(FILE *file, const statement_list_t *stmts)
{
    const statement_t *stmt;
    fprintf(file, "#include \"ifs_thunk.h\"\n");
    if (stmts) LIST_FOR_EACH_ENTRY(stmt, stmts, const statement_t, entry)
    {
        switch (stmt->type)
        {
        case STMT_TYPE:
            if (type_get_type(stmt->u.type) == TYPE_INTERFACE)
            {
                if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
                    write_thunk_1632_3216(file, stmt->u.type);
            }
            break;
        }
    }
}
static void do_write_header_def(FILE *header, const type_t *iface, const char *name)
{
    const statement_t *stmt;
    int first_iface = 1;

    if (type_iface_get_inherit(iface))
        do_write_header_def(header, type_iface_get_inherit(iface), name);

    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
        const var_t *func = stmt->u.var;
        if (first_iface) {
            fprintf(header, "/*** %s methods ***/\n", iface->name);
            first_iface = 0;
        }
        if (!is_callas(func->attrs)) {
            fprintf(header, "    SEGPTR %s;\n", get_name(func));
        }
    }
}

static int is_inherited_method(const type_t *iface, const var_t *func)
{
  while ((iface = type_iface_get_inherit(iface)))
  {
    const statement_t *stmt;
    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
      const var_t *funccmp = stmt->u.var;

      if (!is_callas(func->attrs))
      {
         char inherit_name[256];
         /* compare full name including property prefix */
         strcpy(inherit_name, get_name(funccmp));
         if (!strcmp(inherit_name, get_name(func))) return 1;
      }
    }
  }

  return 0;
}
static int is_override_method(const type_t *iface, const type_t *child, const var_t *func)
{
  if (iface == child)
    return 0;

  do
  {
    const statement_t *stmt;
    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(child))
    {
      const var_t *funccmp = stmt->u.var;

      if (!is_callas(func->attrs))
      {
         char inherit_name[256];
         /* compare full name including property prefix */
         strcpy(inherit_name, get_name(funccmp));
         if (!strcmp(inherit_name, get_name(func))) return 1;
      }
    }
  }
  while ((child = type_iface_get_inherit(child)) && child != iface);

  return 0;
}

static int is_aggregate_return(const var_t *func)
{
  enum type_type type = type_get_type(type_function_get_rettype(func->type));
  return type == TYPE_STRUCT || type == TYPE_UNION ||
         type == TYPE_COCLASS || type == TYPE_INTERFACE;
}

static char *get_vtbl_entry_name(const type_t *iface, const var_t *func)
{
  static char buff[255];
  if (is_inherited_method(iface, func))
    sprintf(buff, "%s_%s", iface->name, get_name(func));
  else
    sprintf(buff, "%s", get_name(func));
  return buff;
}

static void write_forward16(FILE *header, type_t *iface)
{
  fprintf(header, "#ifndef __%s16_FWD_DEFINED__\n", iface->c_name);
  fprintf(header, "#define __%s16_FWD_DEFINED__\n", iface->c_name);
  fprintf(header, "typedef interface %s16 %s16;\n", iface->c_name, iface->c_name);
  fprintf(header, "#endif\n\n" );
}
static void write_forward_decls16(FILE *header, const statement_list_t *stmts)
{
  const statement_t *stmt;
  if (stmts) LIST_FOR_EACH_ENTRY( stmt, stmts, const statement_t, entry )
  {
    switch (stmt->type)
    {
      case STMT_TYPE:
        if (type_get_type(stmt->u.type) == TYPE_INTERFACE)
        {
          if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
            write_forward16(header, stmt->u.type);
        }
        else if (type_get_type(stmt->u.type) == TYPE_COCLASS)
          /*write_coclass_forward(header, stmt->u.type);*/;
        break;
      case STMT_TYPEREF:
      case STMT_IMPORTLIB:
        /* not included in header */
        break;
      case STMT_IMPORT:
      case STMT_TYPEDEF:
      case STMT_MODULE:
      case STMT_CPPQUOTE:
      case STMT_PRAGMA:
      case STMT_DECLARATION:
        /* not processed here */
        break;
      case STMT_LIBRARY:
        /*write_forward_decls(header, stmt->u.lib->stmts);*/
        break;
    }
  }
}

static void write_type_def_or_decl(FILE *f, type_t *t, int field, const char *name)
{
  write_type_v(f, t, field, FALSE, name);
}
static void write_fields16(FILE *h, var_list_t *fields)
{
    unsigned nameless_struct_cnt = 0, nameless_struct_i = 0, nameless_union_cnt = 0, nameless_union_i = 0;
    const char *name;
    char buf[32];
    var_t *v;

    if (!fields) return;

    LIST_FOR_EACH_ENTRY( v, fields, var_t, entry ) {
        if (!v || !v->type) continue;

        switch(type_get_type_detect_alias(v->type)) {
        case TYPE_STRUCT:
        case TYPE_ENCAPSULATED_UNION:
            nameless_struct_cnt++;
            break;
        case TYPE_UNION:
            nameless_union_cnt++;
            break;
        default:
            ;
        }
    }

    LIST_FOR_EACH_ENTRY( v, fields, var_t, entry ) {
        if (!v || !v->type) continue;

        /* indent(h, 0); */
        name = v->name;

        switch(type_get_type_detect_alias(v->type)) {
        case TYPE_STRUCT:
        case TYPE_ENCAPSULATED_UNION:
            if(!v->name) {
                fprintf(h, "__C89_NAMELESS ");
                if(nameless_struct_cnt == 1) {
                    name = "__C89_NAMELESSSTRUCTNAME";
                }else if(nameless_struct_i < 5 /* # of supporting macros */) {
                    sprintf(buf, "__C89_NAMELESSSTRUCTNAME%d", ++nameless_struct_i);
                    name = buf;
                }
            }
            break;
        case TYPE_UNION:
            if(!v->name) {
                fprintf(h, "__C89_NAMELESS ");
                if(nameless_union_cnt == 1) {
                    name = "__C89_NAMELESSUNIONNAME";
                }else if(nameless_union_i < 8 /* # of supporting macros */ ) {
                    sprintf(buf, "__C89_NAMELESSUNIONNAME%d", ++nameless_union_i);
                    name = buf;
                }
            }
            break;
        default:
            ;
        }
        write_type_def_or_decl(h, v->type, TRUE, name);
        fprintf(h, ";\n");
    }
}

static void write_enums16(FILE *h, var_list_t *enums, const char *enum_name)
{
  var_t *v;
  if (!enums) return;
  LIST_FOR_EACH_ENTRY( v, enums, var_t, entry )
  {
    if (v->name) {
      if(!enum_name)
          fprintf(h, "%s16", get_name(v));
      else
          fprintf(h, "%s16_%s16", enum_name, get_name(v));
      if (v->eval) {
        fprintf(h, " = ");
        write_expr(h, v->eval, 0, 1, NULL, NULL, "");
      }
    }
    if (list_next( enums, &v->entry )) fprintf(h, ",\n");
  }
  fprintf(h, "\n");
}
static void write_type_left16(FILE *h, type_t *t, enum name_type name_type, int declonly)
{
    const char *name;

    if (!h) return 0;

    name = type_get_name(t, name_type);
    switch (type_get_type_detect_alias(t)) {
    case TYPE_ENUM:
        if (is_attr(t->attrs, ATTR_CONST) &&
            (type_is_alias(t) || !is_ptr(t)))
            fprintf(h, "const ");
        if (!declonly && t->defined && !t->written) {
            if (name) fprintf(h, "enum %s16 {\n", name);
            else fprintf(h, "enum {\n");
            t->written = TRUE;
            write_enums16(h, type_enum_get_values(t), is_global_namespace(t->namespace) ? NULL : t->name);
            fprintf(h, "}");
        }
        else fprintf(h, "enum %s16", name ? name : "");
        return;
    case TYPE_POINTER:
        if (is_attr(t->attrs, ATTR_CONST) &&
            (type_is_alias(t) || !is_ptr(t)))
            fprintf(h, "const ");
        write_type_left16(h, type_pointer_get_ref(t), name_type, declonly);
        fprintf(h, "%s*", needs_space_after(type_pointer_get_ref(t)) ? " " : "");
        if (is_attr(t->attrs, ATTR_CONST)) fprintf(h, "const ");
        return;
    case TYPE_INTERFACE:
    case TYPE_MODULE:
    case TYPE_COCLASS:
        if (is_attr(t->attrs, ATTR_CONST) &&
            (type_is_alias(t) || !is_ptr(t)))
            fprintf(h, "const ");
        fprintf(h, "%s16", t->name);
        return;
    case TYPE_STRUCT:
        if (is_attr(t->attrs, ATTR_CONST) &&
            (type_is_alias(t) || !is_ptr(t)))
            fprintf(h, "const ");
        if (!declonly && t->defined && !t->written) {
            if (name) fprintf(h, "struct %s16 {\n", name);
            else fprintf(h, "struct {\n");
            t->written = TRUE;
            if (type_get_type(t) != TYPE_STRUCT)
                write_fields16(h, type_encapsulated_union_get_fields(t));
            else
                write_fields16(h, type_struct_get_fields(t));
            //indent(h, -1);
            fprintf(h, "}");
        }
        else
        {
            if (name)
                fprintf(h, "struct %s16", name);
            else
                fprintf(h, "struct ");
        }
        return;

    }
    write_type_left(h, t, name_type, declonly);
}
static int is_object_interface = 0;
static void write_type_v(FILE *h, type_t *t, int is_field, int declonly, const char *name)
{
  type_t *pt = NULL;
  int ptr_level = 0;

  if (!h) return;

  if (t) {
    for (pt = t; is_ptr(pt); pt = type_pointer_get_ref(pt), ptr_level++)
      ;

    if (type_get_type_detect_alias(pt) == TYPE_FUNCTION) {
      int i;
      const char *callconv = get_attrp(pt->attrs, ATTR_CALLCONV);
      if (!callconv && is_object_interface) callconv = "STDMETHODCALLTYPE";
      if (is_attr(pt->attrs, ATTR_INLINE)) fprintf(h, "inline ");
      write_type_left16(h, type_function_get_rettype(pt), NAME_DEFAULT, declonly);
      fputc(' ', h);
      if (ptr_level) fputc('(', h);
      if (callconv) fprintf(h, "%s ", callconv);
      for (i = 0; i < ptr_level; i++)
        fputc('*', h);
    }
    else
      write_type_left16(h, t, NAME_DEFAULT, declonly);
  }

  if (name) fprintf(h, "%s%s16", !t || needs_space_after(t) ? " " : "", name );

  if (t) {
    if (type_get_type_detect_alias(pt) == TYPE_FUNCTION) {
      const var_list_t *args = type_function_get_args(pt);

      if (ptr_level) fputc(')', h);
      fputc('(', h);
      if (args)
          write_args(h, args, NULL, 0, FALSE);
      else
          fprintf(h, "void");
      fputc(')', h);
    } else
      write_type_right(h, t, is_field);
  }
}
static void write_type_def_or_decl16(FILE *f, type_t *t, int field, const char *name)
{
  write_type_v(f, t, field, FALSE, name);
}
static void write_typedef16(FILE *header, type_t *type)
{

    if (type_get_type_detect_alias(type_alias_get_aliasee(type)) == TYPE_UNION || type_get_type_detect_alias(type_alias_get_aliasee(type)) == TYPE_ENCAPSULATED_UNION || type_get_type_detect_alias(type_alias_get_aliasee(type)) == TYPE_STRUCT || type_get_type_detect_alias(type_alias_get_aliasee(type)) == TYPE_ENUM)
        return;
    fprintf(header, "typedef ");
    write_type_def_or_decl16(header, type_alias_get_aliasee(type), FALSE, type->name);
    fprintf(header, ";\n");
}
static void write_method_macro16(FILE *header, const type_t *iface, const type_t *child, const char *name)
{
    const statement_t *stmt;
    int first_iface = 1;

    if (type_iface_get_inherit(iface))
        write_method_macro16(header, type_iface_get_inherit(iface), child, name);

    STATEMENTS_FOR_EACH_FUNC(stmt, type_iface_get_stmts(iface))
    {
        const var_t *func = stmt->u.var;

        if (first_iface)
        {
            fprintf(header, "/*** %s methods ***/\n", iface->name);
            first_iface = 0;
        }

        if (is_override_method(iface, child, func))
            continue;

        if (!is_callas(func->attrs)) {
            const var_t *arg;
            int args_size = 2;

            fprintf(header, "static ");
            write_type_decl_left(header, type_function_get_rettype(func->type));
            fprintf(header, " %s16_%s(SEGPTR This", name, get_name(func));
            if (type_get_function_args(func->type))
            {
                LIST_FOR_EACH_ENTRY(arg, type_get_function_args(func->type), const var_t, entry)
                {
                    int size, is_ptr;
                    get_type_size16(arg->type, &size, &is_ptr);
                    if (size == 4)
                    {
                        const char *n = type_get_name(arg->type, NAME_DEFAULT);
                        /* hard-coded */
                        if (n && !strcmp(n, "SIZE"))
                        {
                            fprintf(header, ",SIZE16 %s", arg->name);
                        }
                        else
                        {
                            fprintf(header, ",DWORD %s", arg->name);
                        }
                        args_size += 2;
                    }
                    else if (size == 8)
                    {
                        const char *n = type_get_name(arg->type, NAME_DEFAULT);
                        /* hard-coded */
                        if (n && !strcmp(n, "POINTL"))
                        {
                            fprintf(header, ",POINTL %s", arg->name);
                        }
                        else
                        {
                            fprintf(header, ",ULARGE_INTEGER %s", arg->name);
                        }
                        args_size += 4;
                    }
                    else
                    {
                        fprintf(header, ",WORD %s", arg->name);
                        args_size++;
                    }
                }
            }
            fprintf(header, ")\n{\n");
            fprintf(header, "    WORD args[] = ");

            if (is_aggregate_return(func))
            {
                fprintf(header, "%s_%s_define_WIDL_C_INLINE_WRAPPERS_for_aggregate_return_support\n", name, get_name(func));
                continue;
            }
            fprintf(header, "{LOWORD(%s)","This");
            fprintf(header, ", HIWORD(%s)", "This");
            if (type_get_function_args(func->type))
            {
                LIST_FOR_EACH_ENTRY(arg, type_get_function_args(func->type), const var_t, entry)
                {
                    int size, is_ptr;
                    get_type_size16(arg->type, &size, &is_ptr);
                    if (size == 4)
                    {
                        const char *n = type_get_name(arg->type, NAME_DEFAULT);
                        /* hard-coded */
                        if (n && !strcmp(n, "SIZE"))
                        {
                            fprintf(header, ", %s.cx", arg->name);
                            fprintf(header, ", %s.cy", arg->name);
                        }
                        else
                        {
                            fprintf(header, ", LOWORD(%s)", arg->name);
                            fprintf(header, ", HIWORD(%s)", arg->name);
                        }
                    }
                    else if (size == 8)
                    {
                        const char *n = type_get_name(arg->type, NAME_DEFAULT);
                        /* hard-coded */
                        if (n && !strcmp(n, "POINTL"))
                        {
                            fprintf(header, ", LOWORD(%s.x)", arg->name);
                            fprintf(header, ", HIWORD(%s.x)", arg->name);
                            fprintf(header, ", LOWORD(%s.y)", arg->name);
                            fprintf(header, ", HIWORD(%s.y)", arg->name);
                        }
                        else
                        {
                            fprintf(header, ", LOWORD(%s.u.LowPart)", arg->name);
                            fprintf(header, ", HIWORD(%s.u.LowPart)", arg->name);
                            fprintf(header, ", LOWORD(%s.u.HighPart)", arg->name);
                            fprintf(header, ", HIWORD(%s.u.HighPart)", arg->name);
                        }
                    }
                    else
                    {
                        fprintf(header, ", %s", arg->name);
                    }
                }
            }
            fprintf(header, "};\n");

            fprintf(header, "    DWORD res;\n");
            fprintf(header, "    WOWCallback16Ex(GET_SEGPTR_METHOD_ADDR(%s16, This, %s), WCB16_PASCAL, %d, args, &res);\n", name, get_vtbl_entry_name(iface, func), args_size * 2);
            fprintf(header, "    return res;\n");
            fprintf(header, "}\n\n");
        }
    }
}
static void write_header16_decl(FILE *file, type_t *iface)
{
    int dispinterface = is_attr(iface->attrs, ATTR_DISPINTERFACE);
    const UUID *uuid = get_attrp(iface->attrs, ATTR_UUID);
    type_t *type;

    const statement_t *stmt;
    const statement_list_t *stmts = type_iface_get_stmts(iface);
    if (!filter_iface(iface->c_name))
        return;
    if (stmts) LIST_FOR_EACH_ENTRY(stmt, stmts, const statement_t, entry)
    {
        switch (stmt->type)
        {
        case STMT_TYPEDEF:
        {
            const type_list_t *type_entry = stmt->u.type_list;
            for (; type_entry; type_entry = type_entry->next)
                write_typedef16(file, type_entry->type);
            break;
        }
        }
    }
    fprintf(file, "typedef struct %s16Vtbl\n{\n", iface->c_name);
    do_write_header_def(file, iface, iface->c_name);
    fprintf(file, "} %s16Vtbl;\n", iface->c_name);
    fprintf(file, "typedef struct %s16\n{\n    /* %s16Vtbl* */SEGPTR lpVtbl;\n", iface->c_name, iface->c_name);
    fprintf(file, "} %s16;\n", iface->c_name);
    type = dispinterface ? type_iface_get_inherit(iface) : iface;
    write_method_macro16(file, type, type, iface->c_name);
}
static void write_header16_decls(FILE *file, const statement_list_t *stmts)
{
    const statement_t *stmt;
    if (stmts) LIST_FOR_EACH_ENTRY(stmt, stmts, const statement_t, entry)
    {
        switch (stmt->type)
        {
        case STMT_TYPE:
            if (type_get_type(stmt->u.type) == TYPE_INTERFACE)
            {
                if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
                {
                    if (is_object(stmt->u.type) || is_attr(stmt->u.type->attrs, ATTR_DISPINTERFACE))
                        write_header16_decl(file, stmt->u.type);
                }
            }
            break;
        }
    }
}

void write_relay16(const statement_list_t *stmts)
{
    FILE *file;
    if (!do_relay16) return;
    if (!(file = fopen(relay16_name, "w"))) {
        error("Could not open %s for output\n", relay16_name);
        return;
    }
    fprintf(file, "/*** Autogenerated by WIDL %s from %s - Do not edit ***/\n\n", PACKAGE_VERSION, input_name);
    if (strendswith(relay16_name, ".h"))
    {
        fprintf(file, "#ifndef __%s__\n", header_token);
        fprintf(file, "#define __%s__\n\n", header_token);
        write_forward_decls16(file, stmts);
        write_header16_decls(file, stmts);
        fprintf(file, "#endif /* __%s__ */\n", header_token);
    }
    else
    {
        write_thunks_1632_3216(file, stmts);
        write_vtable(file, stmts);
        write_vtable2(file, stmts);
    }
    
}
