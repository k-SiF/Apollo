#include "sketch.h"

extern Sketch* createSketch();

int main() {
    Sketch* sketch = createSketch();
    sketch->run();
    delete sketch;
}