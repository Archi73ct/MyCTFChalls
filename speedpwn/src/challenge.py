import random
import os
import subprocess 
with open("template.c", "r") as t:
    template = t.read()
    bufsz = random.randrange(48, 5000)
    bufrd = bufsz + random.randrange(100, 200)

    template = template.replace("__buf_sz__", str(bufsz))
    template = template.replace("__buf_rd__", str(bufrd))

    funcs = ""
    for i in range(random.randrange(2, 55)):
        func = f"""static int f{i}() {{
    int t = {i%5};
    int v = {i};
    return t+v;
}}
"""
        funcs += func
    template = template.replace("__rd_funcs__", funcs)
    open("out.c", "w").write(template)
    subprocess.call(["gcc", "-O0", "-fno-stack-protector", "-fno-pie", "-no-pie", "out.c", "-o", "out"]) 
    
    print(open("out", "rb").read())
    os.execve("./out", ["./out"], dict(os.environ))