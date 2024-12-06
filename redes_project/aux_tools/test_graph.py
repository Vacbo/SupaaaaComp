import networkx as nx
import sys

def main():
    # Check if the file path is provided as an argument
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <path_to_graph_file>")
        sys.exit(1)

    # File path from command-line argument
    file_path = sys.argv[1]

    # Open the file and skip the first line
    try:
        with open(file_path, 'r') as file:
            next(file)  # Skip the first line

            # Read the graph from the remaining lines
            G = nx.parse_adjlist(file)

    except FileNotFoundError:
        print(f"Error: File not found at {file_path}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: Unable to read the file. {e}")
        sys.exit(1)

    # Find all maximal cliques
    maximal_cliques = list(nx.find_cliques(G))

    # Find the maximum clique (largest)
    maximum_clique = max(maximal_cliques, key=len)

    print("Maximal cliques found:")
    for clique in maximal_cliques:
        print(clique)

    print("Maximum clique found:", maximum_clique)


if __name__ == "__main__":
    main()
