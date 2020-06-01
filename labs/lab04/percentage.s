#percentage.s 
#Read in two integers: 
#the total number of marks in the exam, 
#and how many marks the student was awarded.
#Print out what percentage of the marks the student was awarded for that exam
#with no decimal places.

#int max;
#int mark;
#int main(void){
#    printf("Enter the total number of marks in the exam: ");
#    scanf("%d",&max);
#    printf("Enter the number of marks the student was awarded: ");
#    scanf("%d",&mark);
#
#    printf("The student scored %d%% in this exam.\n",100*mark/max);
#    return 0;
#}

          .data
ask_max:  .asciiz "Enter the total number of marks in the exam: "
ask_mark: .asciiz "Enter the number of marks the student was awarded: "


#TODO add more here if needed

      .text
      .globl main
main:
      la    $a0, ask_max
      li    $v0, 4
      syscall                  # printf("Enter the total number of marks in the exam: ");

      #TODO finish implementing code 

      li  $v0, 0        # set return value to 0
      jr  $ra           # return from main

