# weather.s

# int main(void){
#     int temp;
#     int difference;
#     printf("Enter the temperature: ");
#     scanf("%d", &temp);
#     if (temp > 25) {
#         difference = temp - 25;
#         printf("I wish it was %d degrees cooler.\n", difference);
#         if (temp >= 40) {
#             printf("It is boiling!\n");
#         }
#     } else if (temp < 25) {
#         difference = 25 - temp;
#         printf("I wish it was %d degrees warmer.\n", difference);
#         if (temp <= 10) {
#             printf("I am freezing!\n");
#         }
#     } else {
#         printf("Nice weather we are having.\n");
#     } 
#     return 0;
# }


### Global data

   .data

input_msg:
   .asciiz "Enter the temperature: "
wishes_msg:
   .asciiz "I wish it was "
warmer_msg:
   .asciiz " degrees warmer.\n"
cooler_msg:
   .asciiz " degrees cooler.\n"
freezing_msg:
   .asciiz "I am freezing!\n"
boiling_msg:
   .asciiz "I am boiling!\n"
nice_msg:
   .asciiz "Nice weather we are having.\n"


### main() function
   .text
   .globl main

main:
   la    $a0, input_msg
   li    $v0, 4
   syscall                  # printf("Enter a number: ");

   li    $v0, 5
   syscall                  # scanf("%d", into $v0)
   move  $s0, $v0           # store temp in $s0

if_start:
   li    $t0, 25            # store constant 25 in $t0
   blt   $s0, $t0, too_cold
   beq   $s0, $t0, just_right

   li    $v0, 4             # printf("I wish it was ");
   la    $a0, wishes_msg
   syscall

   sub   $t1, $s0, $t0      # diff = $t1 = $s0 - $t0
   
   li    $v0, 1             # printf("%d",$s0);
   move  $a0, $t1   
   syscall

   li    $v0, 4             # printf(" degrees cooler\n");
   la    $a0, cooler_msg
   syscall
  
   li    $t0, 40            # store constant 40 in $t0
   blt   $s0, $t0, end_if
   
   li    $v0, 4             # printf("I am boiling\n");
   la    $a0, boiling_msg
   syscall
  
   j     end_if

too_cold:

   li    $v0, 4             # printf("I wish it was ");
   la    $a0, wishes_msg
   syscall

   sub   $t1, $t0, $s0      # diff = $t1 = $t0 - $s0

   li    $v0, 1             # printf("%d",$t1);
   move  $a0, $t1   
   syscall

   li    $v0, 4
   la    $a0, warmer_msg
   syscall

   li    $t0, 10            # store constant 10 in $t0
   bgt   $s0, $t0, end_if
   
   li    $v0, 4             # printf("I am freezing\n");
   la    $a0, freezing_msg
   syscall

   j     end_if

just_right:

   li    $v0, 4
   la    $a0, nice_msg      # printf("Nice weather we are having.\n");
   syscall

end_if:

   li    $v0, 0
   jr    $ra                # return 0

