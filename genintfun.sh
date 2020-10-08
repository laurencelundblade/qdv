#!/bin/sh

# This is to generate the QCBOR integer decode functions for the different sizes
# It is not done yet

for i in 8 16 32;
do
   for u in "" U; 
   do
     for c in "" Convert ConvertAll; 
     do
        for n in "" InMapN InMapSZ; 
        do
           smallu=`echo $u | tr U u`
           echo static void QCBORDecode_Get${u}Int${i}${c}${n}\(QCBORDecodeContext \*Cptx, uint32_t uOptions, ${smallu}int${i}_t \*puValue\)\;
           echo "{"
           if [ "$c" = "" ] ;
           then 
               echo "    QCBORDecode_GetInt${n}(pMe, QCBOR_CONVERT_TYPE_INT64, puValue);"
           else
               echo "   QCBORItem Item"
               echo "   QCBORDecode_GetInt${n}${c}${n}(QCBORDecodeContext *pMe, nLabel, QCBOR_CONVERT_TYPE_INT64, puValue);"
           fi 
           echo "}"
           echo
           echo
        done
     done
   done
done
