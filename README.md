# DFA_Recognition
check string belong to dfa or not,etc

## Function
- first,input some information about the number of state of a dfa and its move situation etc.
- this program can write into a file to save all above , and it alse can read them.
- you can input a string ,using '#' as end,the program can **check string wether this dfa can accept or not**.
- of course,you can input a nunber and **this dfa can output all string that thier lengths would be beyoud the number you input**.
- some little function such ad judge start state and end states,etc.

## IDE
i use **Visual Studio 2017 Commity**,and the language is **C**.

## Example
### input dfa
2 //the number of character 
a b // character
4 // number of state
0 1 2 3 // state
0 // start state
1 // number of accept
3 // accept
1 2 // state move
3 2
1 3
3 3

### check a string
input a string using '#' as a end
eg: aaaa#
it will output : succeed! 
means this dfa can accept this string.

### show all accept
input a number 'N'
eg: 2
it will output:
aa
bb

# Summary
this is a easy program to recognize 'dfa' or a string.
Thanks for advice to my email:**zpencheng@126.com**.
