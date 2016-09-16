#include "File_reader.h"

extern FILE * fp;

void open_file_stream()
{
    fp = fopen("./iris.data","r");
}
