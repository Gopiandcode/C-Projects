//
// Created by gopia on 01/05/2017.
//

#include "graph.h"

int **matrix(int rows, int columns) {
    int **result = malloc(sizeof(int *)*rows);
    for(int i = 0; i<rows; i++) {
        result[i] = calloc(sizeof(int), columns);
    }
    return result;
}


void delete_path_matrix(path_matrix *path_tree) {
    free(path_tree->nearest);
    free(path_tree->costs);
    free(path_tree);
    path_tree = NULL;
}

void delete_path(path *p) {
    if(p->next == NULL) {
        free(p);
    } else {
        delete_path(p->next);
        free(p);
    }
}

void delete_graph(graph *grp) {
    for(int i = 0; i<grp->nodes; i++) {
        free(grp->graph[i]);
    }
    free(grp->graph);
    free(grp);
}


path_matrix *dfs(graph *grp, int start) {

    if(grp == NULL) {
        fprintf(stderr, "Error: Fatal - Graph not initialized.\n");
        return NULL;
    }

    int *parent = malloc(sizeof(int)*grp->nodes);
    int *cost =  malloc(sizeof(int)*grp->nodes);

    int visited[grp->nodes];
    int onstack[grp->nodes];
    int i, j;

    for(i = 0; i<grp->nodes; i++) {
        visited[i] =  0;
        onstack[i] = 0;
        parent[i] = -1;
        cost[i] = 0;
    }
    int visited_count = 0;
    stack *st =  NULL;
    st = malloc(sizeof(stack));
    st->next = NULL;
    st->node = start;

    while(st != NULL) {

        // Pop the top result off the stack
        stack *current = st;
        int node = current->node;
        st = st->next;
        free(current);
        visited[node] = 1;
        onstack[node] = 0;

        // Find all connected nodes and push onto stack
        for(i = 0; i<grp->nodes; i++) {
            if(grp->graph[node][i] != 0 && !visited[i] && !onstack[i]) {
                parent[i] = node;
                cost[i] = 1 + cost[node];
                onstack[i] = 1;
                stack *new = malloc(sizeof(stack));
                new->next = st;
                new->node = i;
                st = new;
            }
        }

    }


    path_matrix *result = malloc(sizeof(path_matrix));
    result->costs = cost;
    result->nodes = grp->nodes;
    result->nearest = parent;
    result->root = start;


    return result;

}



path_matrix *bfs(graph *grp, int start) {

    if(grp == NULL) {
        fprintf(stderr, "Error: Fatal - Graph not initialized.\n");
        return NULL;
    }

    int *parent = malloc(sizeof(int)*grp->nodes);
    int *cost =  malloc(sizeof(int)*grp->nodes);

    int visited[grp->nodes];
    int onstack[grp->nodes];
    int i, j;

    for(i = 0; i<grp->nodes; i++) {
        visited[i] =  0;
        onstack[i] = 0;
        parent[i] = -1;
        cost[i] = 0;
    }
    int visited_count = 0;
    stack *st =  NULL;
    stack *end = NULL;
    st = malloc(sizeof(stack));
    st->next = NULL;
    st->node = start;

    end = st;

    while(st != NULL) {

        // Dequeue the top result off the queue
        stack *current = st;
        int node = current->node;
        st = st->next;
        if(st == NULL) end = NULL;
        free(current);
        visited[node] = 1;

        // Find all connected nodes and push onto stack
        for(i = 0; i<grp->nodes; i++) {
            if(grp->graph[node][i] != 0 && !visited[i]) {
                parent[i] = node;
                cost[i] = 1 + cost[node];

                stack *new = malloc(sizeof(stack));
                new->next = NULL;
                new->node = i;
                if(end != NULL)
                    end->next = new;
                else
                    st = new;
                end = new;
            }
        }

    }


    path_matrix *result = malloc(sizeof(path_matrix));
    result->costs = cost;
    result->nearest = parent;
    result->nodes = grp->nodes;
    result->root = start;


    return result;

}

graph *generateFlowTo(graph *original, path_matrix *mat, int end) {
    if(mat->nearest[end] == -1) return NULL;


    graph *grp = malloc(sizeof(graph));
    grp->nodes = mat->nodes;
    grp->validated = 0;
    int min = -1;
    int current = end;



    while(end != mat->root) {
        if(min == -1 || min > original->graph[mat->nearest[end]][end])
            min = original->graph[mat->nearest[end]][end];
        end = mat->nearest[end];
    }
    end = current;

    grp->graph = matrix(grp->nodes, grp->nodes);
    while(end != mat->root) {
        grp->graph[mat->nearest[end]][end] = min;
        end = mat->nearest[end];
    }

    return grp;
}


graph *generateSearchGraph(graph *original, path_matrix *mat) {
    graph *grp = malloc(sizeof(graph));
    grp->nodes = mat->nodes;
    grp->validated = 0;
    grp->graph = matrix(grp->nodes, grp->nodes);
    int i;

    for(i = 0; i<grp->nodes; i++) {
        if(mat->nearest[i] != -1) {
            if(original != NULL) {
                grp->graph[mat->nearest[i]][i] = original->graph[mat->nearest[i]][i];
            } else {
                grp->graph[mat->nearest[i]][i] = 1;
            }
        }
    }


    return grp;
}




path_matrix *djikstra(graph *grp, int start) {
    if(grp == NULL) {
        fprintf(stderr, "Error: Fatal - Graph not initialized.\n");
        return NULL;
    }

    int *nearest = malloc(sizeof(int)*grp->nodes);
    int *cost = malloc(sizeof(int)*grp->nodes);
    int *added = malloc(sizeof(int)*grp->nodes);
    path_matrix *result = malloc(sizeof(path_matrix));
    result->root = start;
    result->costs = cost;
    result->nearest = nearest;

    // Initialize the arrays
    int added_nodes = 0;
    int i;
    for(i = 0; i<grp->nodes; i++) {
        nearest[i] = start;
        cost[i] = -1;
        added[i] = 0;
    }
    cost[start] = 0;
    int iterations = 0;


    while(added_nodes != grp->nodes) {
        iterations++;
        if(iterations > DJIKSTRA_LIMIT) {

            fprintf(stderr, "Warning: Djikstra search exceeded iteration limit");
            break;

        }
        // Find the lowest cost
        int min = -1;
        int u;
        for(i = 0; i<grp->nodes; i++) {
            if(!added[i] && cost[i] != -1) {
                if(min == -1 || min > cost[i])
                    min = cost[i], u = i;
            }
        }
        if(min == -1) break;


        // Add it to path
        added[u] = 1;
        added_nodes++;


        // update costs
        for(i = 0; i<grp->nodes; i++) {
            if(!added[i]) {
                if((cost[i] == -1 && grp->graph[u][i] != 0))
                    nearest[i] = u, cost[i] = cost[u] + grp->graph[u][i];
                else if(cost[i] != -1 && grp->graph[u][i] + cost[u] < cost[i]) {
                    cost[i] = grp->graph[u][i] + cost[u];
                    nearest[i] = u;
                }
            }
        }


    }
    result->nodes = grp->nodes;
    return result;
}


path *shortest_path_to_node(path_matrix *path_tree, int to) {

    if(path_tree == NULL) {
        fprintf(stderr, "Error: Fatal - Path Tree not initialized.\n");
        return NULL;
    }


    path *shortest = malloc(sizeof(path));
    shortest->to = to;
    shortest->from = path_tree->nearest[to];
    shortest->next = NULL;
    shortest->prev = NULL;
    to = path_tree->nearest[to];
    while(to != path_tree->root) {
        path *next = malloc(sizeof(path));

        next->next = shortest;
        next->to = to;
        next->from = path_tree->nearest[to];
        next->prev = NULL;

        shortest->prev = next;

        shortest = next;
        to = path_tree->nearest[to];
    }

    return shortest;
}

graph *getData(FILE *fp) {
    // Get the number of nodes
    int nodes, edges;
    fscanf(fp, "N: %d E: %d ", &nodes, &edges);

    if(nodes <= 0) {
        fprintf(stderr, "Input[0]: Invalid data - number of nodes must be positive integer\n");
        return NULL;
    }

    // Get the number of edges
    //fscanf(fp, "", );


    if(edges <= 0) fprintf(stderr, "Input[1]: Invalid data - number of edges must be positive integer\n");

    // Construct Adjacency matrix
    int **adj_mat = matrix(nodes, nodes);

    int i;
    for(i = 0; i<edges && !feof(fp); ++i) {
        int from, to, weight;
        fscanf(fp, "(%d, %d, %d) ", &from, &to, &weight);
        if(from >= nodes || to >= nodes || from < 0 || to< 0 ) fprintf(stderr, "Edge[%d]: Invalid input data - (%d, %d, %d)\n", i, from, to, weight);
        else {
            adj_mat[from][to] = weight;
        }
    }

    if(i != edges) fprintf(stderr, "Error: Could only read %d edges of %d from file.\n", i, edges);

    graph *grp = malloc(sizeof(graph));
    grp->graph = adj_mat;
    grp->nodes = nodes;
    grp->validated = 0;


    return grp;
}

graph *augmentGraph(graph *capacity_graph, graph *f, graph *fstar) {
    if(capacity_graph == NULL || f == NULL || fstar == NULL) {
        fprintf(stderr,"Error: Fatal - Graphs have not been initialized.\n");
        return NULL;
    }

    if(capacity_graph->nodes != f->nodes && capacity_graph->nodes != fstar->nodes) {
        fprintf(stderr,"Error: Fatal - flow graph does not match capacity graph.\n");
        return NULL;
    }

    graph *augmented = malloc(sizeof(graph));
    augmented->nodes = f->nodes;
    augmented->graph = matrix(augmented->nodes, augmented->nodes);
    int i,j;

    for(i = 0;i<f->nodes; i++) {
        for(j = 0; j<f->nodes; j++) {
            int val = 0;

            if(capacity_graph->graph[i][j] != 0) {
                val = f->graph[i][j] + fstar->graph[i][j] - fstar->graph[j][i];
            }
            augmented->graph[i][j] = val;
        }
    }

    return augmented;

}

graph *residualFlowGraph(graph *capacity_graph, graph *flow_graph) {
    if(capacity_graph == NULL || flow_graph == NULL) {
        fprintf(stderr,"Error: Fatal - Graphs have not been initialized.\n");
        return NULL;
    }

    if(capacity_graph->nodes != flow_graph->nodes) {
        fprintf(stderr,"Error: Fatal - flow graph does not match capacity graph.\n");
        return NULL;
    }
    graph *residual = malloc(sizeof(graph));
    residual->nodes = capacity_graph->nodes;
    residual->graph = matrix(residual->nodes, residual->nodes);
    int i, j;
    for(i = 0; i<residual->nodes; i++) {
        for(j = 0; j<residual->nodes; j++) {
            int val = 0;

            if(capacity_graph->graph[i][j] != 0) {
                val = capacity_graph->graph[i][j] - flow_graph->graph[i][j];
            } else if(capacity_graph->graph[j][i] != 0) {
                val = flow_graph->graph[j][i];
            }

            residual->graph[i][j] = val;
        }
    }



    return residual;
}





void printGraph(const char *spec, graph *grp, FILE *fp) {
    if(grp == NULL || fp == NULL) {
        fprintf(stderr,"Error: Fatal - graph or file not initialized.\n");
        return;
    }


    fprintf(fp, "Graph[N:%d]\n", grp->nodes);
    int i, j;
    for(i = 0; i<grp->nodes; ++i) {
        for(j = 0; j<grp->nodes; ++j) {
            if(spec != NULL) {
                fprintf(fp, spec, grp->graph[i][j]);
            } else {
                fprintf(fp, "%3d  ", grp->graph[i][j]);
            }
        }
        fprintf(fp, "\n");
    }
}

void printPath(const char *spec, path *pt, FILE *fp) {
    while(pt != NULL) {
        if(spec == NULL)
            fprintf(fp, "(%d to %d)\n", pt->from, pt->to);
        else
            fprintf(fp, spec, pt->from, pt->to);
        pt = pt->next;
    }

}

int value = 1;
int val = 2;

graph *openGraph(const char *fileName) {
    FILE *fp = fopen(fileName, "r");
    graph *val;
    if(fp != NULL)
        val = getData(fp);
    else
        fprintf(stderr, "Error: Fatal - Could not open file.\n");
    fclose(fp);
    return val;
}


graph *ford_fulkerson(graph *capacity_graph, int source, int sink) {

    // Generate initial flow through graph
    path_matrix *dfs_path = dfs(capacity_graph, source);

    graph *best_flow = generateFlowTo(capacity_graph, dfs_path, sink);

    // Generate residual graph for initial flow
    graph *residual = residualFlowGraph(capacity_graph, best_flow);

    delete_path_matrix(dfs_path);
    dfs_path = dfs(residual, source);

    graph *new_flow = generateFlowTo(capacity_graph, dfs_path, sink);

    while(new_flow != NULL) {

        // Augment best flow with optimization
        graph *betterFlow = augmentGraph(capacity_graph,best_flow, new_flow);


        // Update residual graph
        delete_graph(residual);
        residual = residualFlowGraph(capacity_graph, betterFlow);

        // Update best flow
        delete_graph(best_flow);
        best_flow = betterFlow;



        // Generate path through residual graph
        delete_path_matrix(dfs_path);
        dfs_path = dfs(residual, source);

        delete_graph(new_flow);
        new_flow = generateFlowTo(capacity_graph, dfs_path, sink);

    }


    delete_graph(residual);
    delete_path_matrix(dfs_path);

    return best_flow;

}