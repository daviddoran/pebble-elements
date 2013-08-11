/**
 * Chemical element
 */
typedef struct {
  uint8_t atomic;
  char* symbol;
  char* name;
  char* origin;
  uint8_t group;
  uint8_t period;
  uint8_t weight;
} Element;