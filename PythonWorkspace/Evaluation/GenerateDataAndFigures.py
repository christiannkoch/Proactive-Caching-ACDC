import os
def exec_full(filepath):
    import os
    global_namespace = {
        "__file__": filepath,
        "__name__": "__main__",
    }
    with open(filepath, 'rb') as file:
        exec(compile(file.read(), filepath, 'exec'), global_namespace)

# execute the file
if(len(os.listdir('SimulationResults/Top1')) > 1):
	exec_full("GenerateTopology1Data.py")
if(len(os.listdir('SimulationResults/Top2')) > 1):
	exec_full("GenerateTopology2Data.py")
if(len(os.listdir('SimulationResults/Top3')) > 1):
	exec_full("GenerateTopology3Data.py")
if(len(os.listdir('SimulationResults/Top4')) > 1):
	exec_full("GenerateTopology4Data.py")
if(len(os.listdir('SimulationResults/Top1')) > 1):
	exec_full("GenerateTopology1Figures.py")
if(len(os.listdir('SimulationResults/Top2')) > 1):
	exec_full("GenerateTopology2Figures.py")
if(len(os.listdir('SimulationResults/Top3')) > 1):
	exec_full("GenerateTopology3Figures.py")
if(len(os.listdir('SimulationResults/Top4')) > 1):
	exec_full("GenerateTopology4Figures.py")