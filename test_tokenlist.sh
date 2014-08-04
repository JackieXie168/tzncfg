#!/bin/bash

if [ ! -e tokenlist ]; then
	./autogen.sh && make
fi

./tokenlist -s "" -a 1 -d ":" -p 1
./tokenlist -s 0 -a 1 -d ":" -p 1
./tokenlist -s 0 -a 1 -d ":" -p 2
val="1 2 3 4 5 6 7 8 9 10" && echo $val
./tokenlist -s "$val" -e 11 -d " "
./tokenlist -s "$val" -e 10 -d " "
val="`./tokenlist -s "$val" -o ":" -d " "`" && echo $val
val="`./tokenlist -s "$val" -o "::" -d ":"`" && echo $val
val="`./tokenlist -s "$val" -o "::" -d "::"`" && echo $val
val="`./tokenlist -s "$val" -o ":::" -d "::"`" && echo $val
val="`./tokenlist -s "$val" -o "::" -d ":::"`" && echo $val
val="`./tokenlist -s "$val" -o ":" -d "::"`" && echo $val
val="`./tokenlist -s "$val" -a 0 -d ":" -p 1`" && echo $val
val="`./tokenlist -s "$val" -a 11 -d ":" -p -8`" && echo $val
val="`./tokenlist -s "$val" -m 11 -d ":" -p 1`" && echo $val
val="`./tokenlist -s "$val" -o "%%" -d ":"`" && echo $val
val="`./tokenlist -s "$val" -a 21 -d "%%" -p 11`" && echo $val
val="`./tokenlist -s "$val" -o ":::" -d "%%"`" && echo $val
./tokenlist -s "$val" -i 3 -d":::"
val="`./tokenlist -s "$val" -o "----" -d ":::"`" && echo $val
./tokenlist -x "$val" -d "----"
val="`./tokenlist -s "$val" -a 100 -d "----" -p 5`" && echo $val
val="`./tokenlist -s "$val" -o "::::" -d "----"`" && echo $val
val="`./tokenlist -s "$val" -d "::::" -r 5`" && echo $val
val="`./tokenlist -s "$val" -o "^_^||" -d "::::"`" && echo $val
val="`./tokenlist -s "$val" -d "^_^||" -r -6`" && echo $val
val="`./tokenlist -s "$val" -o "::::::" -d "^_^||"`" && echo $val
./tokenlist -s "$val" -d "::::::" -v 7
val="`./tokenlist -s "$val" -o "::::::" -d "::::"`" && echo $val
val="1 22 333 4444 55555 666666 7777777 88888888 999999999 10101010101010101010" && echo $val
val="`./tokenlist -s "$val" -o "::::::" -d " "`" && echo $val
val="`./tokenlist -s "$val" -m 1111111111111111111111 -d "::::::" -p 9`" && echo $val
val="`./tokenlist -s "$val" -a 121212121212121212121212 -d "::::::" -p 12`" && echo $val
val="`./tokenlist -s "$val" -o "--" -d "::::::"`" && echo $val
val="`./tokenlist -s "$val" -a 0 -d "--" -p 10`" && echo $val
./tokenlist -s "$val" -d "--" -v 0
val="`./tokenlist -s "$val" -m 0 -d "--" -p 10`" && echo $val
val="`./tokenlist -s "$val" -d "--" -r 10`" && echo $val
val="`./tokenlist -s "$val" -o "----" -d "--"`" && echo $val
val="`./tokenlist -s "$val" -a 0 -d "----" -p 11`" && echo $val
val="`./tokenlist -s "$val" -d "----" -r 11`" && echo $val
val="`./tokenlist -s "$val" -o "-" -d "----"`" && echo $val
val="`./tokenlist -s "$val" -a 0 -d "-" -p 1`" && echo $val
val="`./tokenlist -s "$val" -o "~~" -d "-"`" && echo $val
val="`./tokenlist -s "$val" -m 1111111111111111111111 -d "~~" -p 10`" && echo $val
val="`./tokenlist -s "$val" -o "~~~" -d "~~"`" && echo $val
val="`./tokenlist -s "$val" -d "~~~" -r 9`" && echo $val
val="`./tokenlist -s "$val" -o "*_*" -d "~~~"`" && echo $val
val="`./tokenlist -s "$val" -d "*_*" -r 1`" && echo $val
val="`./tokenlist -s "$val" -o "_*_" -d "*_*"`" && echo $val
val="`./tokenlist -s "$val" -d "_*_" -r 10`" && echo $val
val="`./tokenlist -s "$val" -o "|_|_|_|" -d "_*_"`" && echo $val
val="`./tokenlist -s "$val" -d "|_|_|_|" -r 11`" && echo $val
val="`./tokenlist -s "$val" -o "+++" -d "|_|_|_|"`" && echo $val
./tokenlist -s "$val" -d "+++" -e 1
val="`./tokenlist -s "$val" -o "/\/\" -d "+++"`" && echo $val
./tokenlist -s "$val" -d "/\/" -e 88888888
val="`./tokenlist -s "$val" -o "///" -d "/\/"`" && echo $val
./tokenlist -s "$val" -d "///" -e 10101010101010101010
val="`./tokenlist -s "$val" -o "-#|#-" -d "///"`" && echo $val
val="`./tokenlist -s "$val" -a 0 -d "-#|#-" -p 1`" && echo $val
val="`./tokenlist -s "$val" -d "-#|#-" -n 6`" && echo $val
val="0 11 222 3333 44444 555555 6666666 77777777 888888888 9999999999 10101010101010101010" && echo $val
val="`./tokenlist -s "$val" -m -999999999 -d " " -p 10`" && echo $val
val="`./tokenlist -s "$val" -m -10101010101010101010 -d " " -p 11`" && echo $val
./tokenlist -s "$val" -d " " -v 44444

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -d " " -p $i`; val="$str"; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -p $i -d ":::::"`; val="$str"; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -p $i -d ":::::"`; val="$str"; echo $str; done && echo "Get tokens from list ..." && for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -i $i -d ":::::"`; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -p $i -d "::::::"`; val="$str"; echo $str; done && echo "Modify tokens from list ..." && for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -m $i -p $(expr 51 - $i) -d "::::::"`; val="$str"; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -p $i -d ":::::"`; val="$str"; echo $str; done && echo "Modify tokens from list ..."  && for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -m $(expr 51 - $i) -p $i -d ":::::"`; val="$str"; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $i -p $i -d "::::"`; val="$str"; echo $str; done && echo "Delete tokens from list ..." && for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -r $(expr 51 - $i) -d "::::"`; val="$str"; echo $str; done

echo "" && echo "Inserting tokens ..." && val="";for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -a $(expr 51 - $i) -p $i -d "::::::"`; val="$str"; echo $str; done && echo "Reverse token list ..." && for((i=1;i<=50;i++)); do str=`./tokenlist -s "$val" -m $i -p $i -d "::::::"`; val="$str"; echo $str; done

echo "" && echo "To get total counts of tokens from list ..." && ./tokenlist -x "The program we dissect reads text from the standard input, modifies it, and writes it to the standard output." -d " "

echo "" && echo "To get tokens from list ..." && val="Dealing with character strings is a common task in C programs." && cnt=`./tokenlist -x "$val" -d " "` && for((i=1;i<=cnt;i++)); do str=`./tokenlist -s "$val" -i $i -d " "`; echo $str; done

echo "" && echo "Outputs the tokens in reverse order for the string : 'This is, the test data for the program'"
val="This is, the test data for the program";i=`./tokenlist -x "$val" -d ","`;for((j=$i;j>=1;j--)); do str=`./tokenlist -s "$val" -i $j -d ","`; str=`./trim "$str"`; k=`./tokenlist -x "$str" -d " "`; for((c=$k;c>=1;c--)); do res=`./tokenlist -s "$str" -i $c -d " "`; if [ ! -z "$str" ]; then echo $res; fi; done; done
echo
str="0 11 222 3333 44444 555555 6666666 77777777 888888888 9999999999 1010101010101010101010"
str="`./tokenlist -s "$str" -o "*" -d " "`" && echo $str
str="`./tokenlist -s "$str" -o "&&" -d "*"`" && echo $str
str="`./tokenlist -s "$str" -o "---" -d "&&"`" && echo $str
str="`./tokenlist -s "$str" -o "^^^" -d "---"`" && echo $str
str="`./tokenlist -s "$str" -o "~v~" -d "^^^"`" && echo $str
str="`./tokenlist -s "$str" -o " " -d "~v~"`" && echo $str
str="`./tokenlist -s "$str" -o "----" -d " "`" && echo $str
str="`./tokenlist -s "$str" -o "{[]}" -d "----"`" && echo $str
str="`./tokenlist -s "$str" -o "~*~" -d "{[]}"`" && echo $str
str="`./tokenlist -s "$str" -o "~*~" -d "~*~"`" && echo $str
str="`./tokenlist -s "$str" -o ":::" -d "~*~"`" && echo $str
str="`./tokenlist -s "$str" -o "**&&" -d ":::"`" && echo $str
str="`./tokenlist -s "$str" -o "###" -d "**&&"`" && echo $str
str="`./tokenlist -s "$str" -o "@@@" -d "###"`" && echo $str
str="`./tokenlist -s "$str" -o "|||" -d "@@@"`" && echo $str
str="`./tokenlist -s "$str" -o ";;;" -d "|||"`" && echo $str
str="`./tokenlist -s "$str" -o "~@~" -d ";;;"`" && echo $str
str="`./tokenlist -s "$str" -o "~v~^~^~" -d"~@~"`" && echo $str
str="`./tokenlist -s "$str" -o " " -d"~v~^~^~"`" && echo $str
str="`./tokenlist -s "$str" -o "--" -d" "`" && echo $str
str="`./tokenlist -s "$str" -o "---" -d"--"`" && echo $str
str="`./tokenlist -s "$str" -o "----" -d"---"`" && echo $str
str="`./tokenlist -s "$str" -o "--" -d"----"`" && echo $str
str="`./tokenlist -s "$str" -o "-" -d"--"`" && echo $str
str="`./tokenlist -s "$str" -o " " -d"-"`" && echo $str && echo $str > tmp
./tokenlist -s "`cat tmp`" -o "  " -d" " > tmp && cat tmp
./tokenlist -s "`cat tmp`" -o "   " -d"  " > tmp && cat tmp
./tokenlist -s "`cat tmp`" -o "    " -d"   " > tmp && cat tmp
./tokenlist -s "`cat tmp`" -o "   " -d"    " > tmp && cat tmp
./tokenlist -s "`cat tmp`" -o "  " -d"   " > tmp && cat tmp
./tokenlist -s "`cat tmp`" -o " " -d"  " > tmp && str=`cat tmp` && echo $str
str="`./tokenlist -s "$str" -o "~" -d" "`" && echo $str
str="`./tokenlist -s "$str" -o " " -d"~"`" && echo $str
