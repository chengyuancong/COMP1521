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
#     printf("Enter the number of marks the student was awarded: ");
#     scanf("%d",&mark);
#
#     printf("The student scored %d%% in this exam.\n",100*mark/max);
#     return 0;
# }

            .data
max:        .space 4
mark:       .space 4        
ask_max:    .asciiz "Enter the total number of marks in the exam: "
ask_mark:   .asciiz "Enter the number of marks the student was awarded: "
            .text
main:
      la    $a0, ask_max
      li    $v0, 4
      syscall                  # printf("Enter the total number of marks in the exam: ")
      li    $v0, 5
      syscall                  # scanf("%d", &max)
      sw    $v0, max           # max = reg[v0]

      la    $a0, ask_mark
      li    $v0, 4
      syscall                  # printf("Enter the number of marks the student was awarded: ")
      li    $v0, 5
      syscall                  # scanf("%d", &mark)
      sw    $v0, mark          # mark = reg[v0]

      lw    $t0, mark          # reg[t0] = mark
      li    $t1, 100           # reg[t1] = 100
      mult  $t0, $t1           # 100 * mark
      mfhi  $t0                # reg[t0] = Hi
      lw    $t1, max
      div   $t0, $t1           # (100 * mark) / max
      mflo  $t0                # reg[t0] = Lo

      move  $a0, $t0           # reg[a0] = reg[t0]
      li    $v0, 1             
      syscall                  # printf("%d", t0)
      
      li  $v0, 0        # set return value to 0
      jr  $ra           # return from main


