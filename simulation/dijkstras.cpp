#include "dijkstras.h"
#include <QTime>

Dijkstras::Dijkstras(Graph *graph) {
    QTime t;
    t.start();
    build(graph);
    qDebug("Dijkstras: built table for %i nodes in %d ms", graph->edges.size(), t.elapsed());
}

struct Node {
    float dist;
    Intersection *n;
};

bool operator <(const Node &leftNode, const Node &rightNode) {
    if(leftNode.dist != rightNode.dist) return leftNode.dist > rightNode.dist;
    if(leftNode.n != rightNode.n) return leftNode.n->num < rightNode.n->num;
    return false;
}

// This will be run in a seperate thread
void Dijkstras::build(Graph *graph) {
    typedef gpriority_queue< gheap<4, 1>, Node> priority_queue;
    int count = Intersection::count;

    table = new float[count * count];   
    for(int i=0; i < count * count; i++) // Init table to infinity
        table[i] = INFINITY;

    char visited[count];

    for(auto start : graph->nodes) {
        memset(&visited, 0, Intersection::count);
        priority_queue queue;
        queue.push(Node{0.0, start});
        int col = start->num * count;

        while(!queue.empty()) {
            Node n = queue.top();
            queue.pop();
            int num = n.n->num;
            if(visited[num]) continue;
            for( Road *r : n.n->edges) {
                Intersection *next = r->start != n.n ? r->start : r->end;
                if(!visited[next->num])
                    queue.push(Node{n.dist + r->length, next});
            }
            visited[num] = 1;
            table[col + num] = n.dist;
        }
    }
}
