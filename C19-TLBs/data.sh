if [[ -z "$1" ]] 
  then
   echo "arg1 is missing"
   echo "arg1: maxpagesize = 2^arg1"
   exit 1
fi
echo "start processing..."

rm data

PAGESIZE=$((2**$1))

COUNTER=1
while [  $COUNTER -lt $PAGESIZE ]; do
 ./tlb $COUNTER 1000 >> data
 let COUNTER=COUNTER*2
done

echo "Done"
