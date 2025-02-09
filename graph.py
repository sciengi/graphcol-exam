import subprocess
import random

class graph:

    def __init__(self, adj):
        self.adj = adj  # copy!

    def plot(self, filename, *args, names=None, nc=0, directed=False, weighted=False, weights=None, layout_engine='dot'):
        '''
        Plots by graphviz tools graph given as adjacency matrix.

        Parameters
        ----------
        filename 
            Filename to save as svg.

        directed
           Boolean that shows how draw matrix: as directed/undirected graph.
        
        weighted
            Boolean that shows draw/dont draw weight on edge.

        nc
            Element that represent no connection.

        layout_engine
            Any graphviz tool (dot, neato, circo, etc).
        
        '''

        # TODO:
        # - Cant it be without file?
        # - File io and extension hdl
        # - Write edgeless nodes
        # - Make new pipeline

        shape_type = 'circle' if names else 'point'
        if names == None:
            names = range(len(self.adj[0]))

        if weights == None:
            weights = self.adj

        arrow_type = '->' if directed else '--'
        graph_type = 'digraph' if directed else 'graph'
        

        with open('gsrc.dot', 'w') as f:
            f.write(f'{graph_type}' ' {' f'node [shape={shape_type}]; ')

            for i in range(len(self.adj[0])):
                start = 0 if directed else i
                for j in range(start, len(self.adj[0])):

                    if self.adj[i][j] != nc:
                        f.write(f'{names[i]} {arrow_type} {names[j]} ')
                        if weighted:
                            f.write(f'[label={weights[i][j]}]; ')
                            
            f.write('}')

        args = [layout_engine, '-Tsvg', f'-o{filename}', 'gsrc.dot', *args]
        subprocess.run(args)


# TODO: 
# - another graphs by same method thought arguments
# - remove (example purpose only)
def gen_Heawood():
	''' Generate [Heawood graph](https://en.wikipedia.org/wiki/Heawood_graph). '''
	adj = [[0 for _ in range(14)] for _ in range(14)]

	sign = 1
	for i in range(14):
		left = (i - 1) % 14
		right = (i + 1) % 14
		mid = (i + sign * 5) % 14
		sign = -sign

		adj[i][left] = 1
		adj[i][mid] = 1
		adj[i][right] = 1

	return adj


def randomize_Heawood(mat, p=0.7):
	for i in range(len(mat[0])):
		for j in range(len(mat[0])):
		    if mat[i][j] == 1 and p > random.random():
		        mat[i][j] = 0


if __name__ == '__main__':

	G_Heawood = graph(gen_Heawood())
	G_Heawood.plot('graph_Heawood.svg', '-Nfontcolor=blue', names=range(14), layout_engine='circo')

	randomize_Heawood(G_Heawood.adj, p=0.5)
	G_Heawood.plot('graph_randomed_directed_Heawood.svg', directed=True, layout_engine='neato')

