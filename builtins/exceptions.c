void def_NameError(environment *env)
{
    NameError.type = pyclass_t;
    NameError.class = &type_class;
    NameError.id = "NameError";
    NameError.env = environment_init(0);
    NameError.inheritance = 0;
    store_id(env, "NameError", &NameError);
}