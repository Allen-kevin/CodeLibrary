#include "ssl.h"

void tls_init()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}
