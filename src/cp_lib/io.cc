
#include "io.h"

namespace cp 
{

i64 
file_size(FILE* file) {
    i64 init_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    i64 size = ftell(file);
    fseek(file, init_pos, SEEK_SET);
    return size;
}

dstrb 
read(const char* file_name) 
{
    FILE* file = fopen(file_name, "r");
    if (file == null) {
        // set error flag
        return {};
    }

    i64 fsize = file_size(file);
    dstrb sb;
    init(&sb, fsize);
    sb.len = fsize;
    fread(beginp(sb), sizeof(u8), cap(sb), file);
    fclose(file);

    return sb;
}


} // namespace cp
