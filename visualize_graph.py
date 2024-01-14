import json
import networkx as nx
import matplotlib.pyplot as plt

def load_graph(file_path):
    """
    Loads graph data from a JSON file and creates a NetworkX graph.
    Args:
    file_path (str): The path to the JSON file containing the graph data.
    Returns:
    networkx.Graph: A graph object containing nodes and edges with weights.
    """
    with open(file_path, 'r') as file:
        data = json.load(file)
    G = nx.Graph()
    for node in data['nodes']:
        G.add_node(node['id'], label=node['label'])
    for edge in data['edges']:
        G.add_edge(edge['from'], edge['to'], weight=edge['weight'])
    return G

def draw_graph(G):
    """
    Draws the graph using matplotlib.
    Args:
    G (networkx.Graph): The graph to be drawn.
    """
    # Use a layout that spreads nodes nicely and avoids overlap
    pos = nx.spring_layout(G, k=0.15, iterations=20)

    # Draw nodes with different colors for students and teachers
    student_nodes = [node for node, data in G.nodes(data=True) if data['label'][:3] != 'Sir' and data['label'][:4] != 'Maam']
    teacher_nodes = [node for node, data in G.nodes(data=True) if data['label'][:3] == 'Sir' or data['label'][:4] == 'Maam']
    
    nx.draw_networkx_nodes(G, pos, nodelist=student_nodes, node_color='skyblue', node_size=400, alpha=0.9)
    nx.draw_networkx_nodes(G, pos, nodelist=teacher_nodes, node_color='lightgreen', node_size=500, alpha=0.9)
    
    # Draw edges with a subtle color and style
    nx.draw_networkx_edges(G, pos, edge_color='gray', width=2, style='dashed')

    # Label nodes with their names
    labels = nx.get_node_attributes(G, 'label')
    nx.draw_networkx_labels(G, pos, labels=labels, font_size=10)

    # Add edge weights as labels on the edges
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='red')

    # Remove the axis
    plt.axis('off')

    # Set the size of the plot
    plt.gcf().set_size_inches(12, 12)

    # Save the plot as a high-quality PNG image
    plt.savefig('graph_visualization.png', format='PNG', dpi=300)

    # Display the plot
    plt.show()

if __name__ == "__main__":
    graph = load_graph("graph_data.json")  # Replace with the path to your JSON file
    draw_graph(graph)
