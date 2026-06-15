#include <apollo/sketch.h>

extern apollo::Sketch* createSketch();

int main() {
    apollo::Sketch* sketch = createSketch();
    sketch->run();
    delete sketch;
}