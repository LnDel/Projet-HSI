import json

with open("types_and_data.json", "r") as file:
    data = json.load(file)

def generate_c_code(data):

    include = ["#include <stdint.h>"]

    types = data["types"]
    datas = data["datas"]

    type_defs = []
    for type in types:
        if type["type"] == "atom":
            type_defs.append(f"typedef {type['declaration']} {type['name']};")
        elif type["type"] == "enum":
            enum_values = ", ".join([f"{k} = {v}" for k, v in type["enum"].items()])
            type_defs.append(f"typedef enum \n {{ {enum_values} }} {type['name']};")

    struct_fields = [f"    {data['type']} {data['name']};" for data in datas]
    struct_def = f"typedef struct {{\n{chr(10).join(struct_fields)}\n}} BCGV_Data_t;"

    instantiation = "static BCGV_Data_t bcgv_data;"

    getters = [f"{data['type']} get_{data['name']}(void) {{ return bcgv_data.{data['name']}; }}" for data in datas]

    setters = []
    for data in datas:
        domain = next((type["domain"] for type in types if type["name"] == data["type"]), None)
        if domain:
            condition = []
            if "min" in domain:
                condition.append(f"value >= {domain['min']}")
            if "max" in domain:
                condition.append(f"value <= {domain['max']}")
            condition_str = " && ".join(condition)
            setters.append(f"""short int set_{data['name']}({data['type']} value) {{
    if ({condition_str}) {{
        bcgv_data.{data['name']} = value;
        return 1;
    }}
    return 0;
}}""")

    initializations = [f"    bcgv_data.{data['name']} = {data['initValue']};" for data in datas]
    init_function = f"void init_BCGV_Data(void) {{\n{chr(10).join(initializations)}\n}}"

    code = "\n\n".join(include + type_defs + [struct_def, instantiation] + getters + setters + [init_function])
    return code

c_code = generate_c_code(data)

with open("bcgv_lib.c", "w") as file:
    file.write(c_code)

print("Succes")
