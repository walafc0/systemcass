
#define ASSERT(x)                    \
{                                    \
    if (!(x)) {                      \
        cerr << "assert failed line " << __LINE__ << ", file " << __FILE__ << ": #x\n"; \
        exit(1);                     \
    }                                \
}

