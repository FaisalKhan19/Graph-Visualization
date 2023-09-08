// Initialize a variable to keep track of the current path index
let currentPathIndex = 0;

async function read_outputs() {
    const pythonData = await eel.read_output()();
    const augmented_paths = pythonData[0];
    const flows = pythonData[1];
    console.log(augmented_paths, flows);

    const next = document.getElementById("next");
    const refresh = document.getElementById("refresh"); // Get the refresh button
    const infoPaths = document.getElementById("info-paths"); // Get the parent element for info
    const infoTotal = document.getElementById('total-flow');
    const total_flow = document.createElement('span');
    next.addEventListener('click', function () {
        // Check if there are more paths to highlight
        if (currentPathIndex < augmented_paths.length) {
            print_info(augmented_paths[currentPathIndex], flows[currentPathIndex], infoPaths);
            // Reset the graph colors before highlighting the next path
            reset_graph();
            update_graph(augmented_paths[currentPathIndex], flows[currentPathIndex]);

            const totalFlow = flows.slice(0, currentPathIndex + 1).reduce((acc, cur) => acc + cur, 0);
            total_flow.textContent = totalFlow;
            infoTotal.appendChild(total_flow);
            currentPathIndex++;
        }
    });

    refresh.addEventListener('click', function () {
        // Reset the current path index and clear the info paths
        currentPathIndex = 0;
        infoPaths.innerHTML = "";
        // Reset the graph colors
        reset_graph();
    });
}

function print_info(path, flow, parent) {
    // Create text elements for path, flow, and space
    const pathText = document.createElement('span');
    const flowText = document.createElement('span');
    const space = document.createElement('span');
    const lineBreak = document.createElement('br'); // Add a line break
    pathText.style.paddingBottom = '5px';
    flowText.style.paddingBottom = '5px';
    space.style.paddingBottom = '5px';
    // Set the text content for the path and flow
    pathText.textContent = path;
    flowText.textContent = flow;
    space.textContent = " => ";

    // Append the text elements and line break to the parent element
    parent.appendChild(pathText);
    parent.appendChild(space);
    parent.appendChild(flowText);
    parent.appendChild(lineBreak); // Add a line break after each set of elements
}

// Function to reset node and edge colors
function reset_graph() {
    svg.selectAll(".node")
        .style("fill", "lightblue");
    svg.selectAll(".edge")
        .style("stroke", "grey");
    svg.selectAll(".edge-arrow")
        .style("stroke", "grey");
}

// Function to update edge colors based on the current augmented path
function update_graph(path, flow) {
    // Highlight nodes and edges along the path
    for (let i = 0; i < path.length; i++) {
        // Highlight nodes
        const nodeId = path[i];
        svg.select(`#node-${nodeId}`)
            .style("fill", "red");

        // Highlight edges
        if (i < path.length - 1) {
            const source = path[i];
            const target = path[i + 1];
            svg.select(`#edge-${source}-${target}`)
                .style("stroke", "red");
        }
    }
}

read_outputs();
