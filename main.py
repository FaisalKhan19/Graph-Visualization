import eel
import re

eel.init('templates')
# Read the output file

@eel.expose
def read_output():
    print("Reading file output.txt")
    with open("output.txt", "r") as file:
        output_content = file.read()

    # Use regular expressions to extract augmented paths and flows
    augmented_path_pattern = re.compile(r"Augmented path chosen : (.+)\nFlow added in this augmenting path : (\d+)")
    augmented_path_matches = augmented_path_pattern.findall(output_content)

    # Format the augmented paths and flows
    augmented_paths = []
    flows = []
    for path, flow in augmented_path_matches:
        len_ = len(path.split(' -> '))
        path = [int(i) for i in path.split(' -> ')[:len_-1]]
        augmented_paths.append(path)
        flows.append(int(flow))
    print(augmented_paths, flows)
    return augmented_paths,flows

eel.start('index.html',size=(800,600))

