#include "../inc/qcbor/qcbor_common.h"
#include <stdio.h>

int main()
{
   for(int i = 0; i < 50; i++) {
      puts(qcbor_err_to_str(i));
   }
}
