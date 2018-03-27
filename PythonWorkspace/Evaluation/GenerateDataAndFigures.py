def exec_full(filepath):
    import os
    global_namespace = {
        "__file__": filepath,
        "__name__": "__main__",
    }
    with open(filepath, 'rb') as file:
        exec(compile(file.read(), filepath, 'exec'), global_namespace)

# execute the file

exec_full("GenerateTopology1Data.py")
exec_full("GenerateTopology2Data.py")
exec_full("GenerateTopology3Data.py")
exec_full("GenerateTopology4Data.py")
exec_full("GenerateTopology1Figures.py")
exec_full("GenerateTopology2Figures.py")
exec_full("GenerateTopology3Figures.py")
exec_full("GenerateTopology4Figures.py")