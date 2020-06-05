# asterisks.s
#
# int main(void){
#     int i, n;
#     printf("Enter the number of asterisks: ");   
#     scanf("%d",&n);
#     i = 0;
#     while(i < n){
#         printf("*\n");
#         i++;
#     }
#     return 0;
# }

### Global data

   .data

input_msg:
   .asciiz "Enter the number of asterisks: "
eol:
   .asciiz "\n"
asterisk:
   .asciiz "*"


### main() function
   .text
   .globl main

main:
   la    $a0, input_msg
   li    $v0, 4
   syscall                  # printf("Enter the number of asterisks: ");

   li    $v0, 5
   syscall                  # scanf("%d", into $v0)
   move  $s0, $v0           # store n in $s0

   li    $t0, 0             # i = 0

loop:
   bge   $t0, $s0, end       # if (i >= n) goto end

   la    $a0, asterisk 
   li    $v0, 4
   syscall                  # printf("*")
   la    $a0, eol
   li    $v0, 4
   syscall                  # printf("\n")

   add   $t0, $t0, 1        # i++
   j     loop

end:
   li    $v0, 0
   jr    $ra                # return 0

