# percentage.s 
# Read in two integers: 
# the total number of marks in the exam, 
# and how many marks the student was awarded.
# Print out what percentage of the marks the student was awarded for that exam
# with no decimal places.

# int max;
# int mark;
# int main(void){
#     printf("Enter the total number of marks in the exam: ");
#     scanf("%d",&max);
#     if (max == 0) goto error;
#           printf("Enter the number of marks the student was awarded: ");
#           scanf("%d",&mark);
#           printf("The student scored %d%% in this exam.\n",100*mark/max);
#     goto end;
# error:
#           printf("Total mark is wrong.\n");
# end:
#     return 0;
# }

            .data
max:        .space 4
mark:       .space 4        
ask_max:    .asciiz "Enter the total number of marks in the exam: "
ask_mark:   .asciiz "Enter the number of marks the student was awarded: "
result_1:   .asciiz "The student scored "
result_2:   .asciiz "% in this exam.\n"
error:      .asciiz "Error: Total mark should be larger than 0.\n"

            .text
            .globl main
main:
      la    $a0, ask_max       # reg[a0] = ask_max
      li    $v0, 4
      syscall                  # printf("Enter the total number of marks in the exam: ")
      li    $v0, 5
      syscall                  # scanf("%d", &max)
      sw    $v0, max           # max = reg[v0]

      lw    $t0, max           # reg[t0] = max
      beq   $t0, 0, wrong      # if (max == 0) goto wrong

      la    $a0, ask_mark      # reg[a0] = ask_mark
      li    $v0, 4
      syscall                  # printf("Enter the number of marks the student was awarded: ")
      li    $v0, 5
      syscall                  # scanf("%d", &mark)
      sw    $v0, mark          # mark = reg[v0]

      lw    $t0, mark          # reg[t0] = mark
      mul   $t0, $t0, 100      # reg[t0] = mark * 100
      lw    $t1, max           # reg[t1] = max
      div   $t0, $t0, $t1      # reg[t0] = (100 * mark) / max


      la    $a0, result_1      # reg[a0] = result_1
      li    $v0, 4
      syscall                  # printf("The student scored ")
      move  $a0, $t0           # reg[a0] = reg[t0]
      li    $v0, 1             
      syscall                  # printf("%d")
      la    $a0, result_2      # reg[a0] = result_2
      li    $v0, 4
      syscall                  # printf("% in this exam.\n")

      b     end                # goto end

wrong:
      la    $a0, error         # reg[a0] = error
      li    $v0, 4 
      syscall                  # printf("Error: Total mark should be larger than 0.\n")           

end:    
      li  $v0, 0               # set return value to 0
      jr  $ra                  # return from main


