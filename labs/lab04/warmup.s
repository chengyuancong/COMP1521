# warmup.s ... add two numbers interactively
# 
# int x;
# int y;
# void main(void) {
#    printf("%s","First number: ");
#    scanf("%d", &x);
#    printf("%s","Second number: ");
#    scanf("%d", &y);
#    printf("%d\n", x+y);
#    return;
# }

      .data
x:    .space 4
y:    .space 4
ask1: .asciiz "First number: "
ask2: .asciiz "Second number: "
eol:  .asciiz "\n"     # char *eol = "\n";

      .text
main:
      la   $a0, ask1    # reg[a0] = &ask1
      li   $v0, 4
      syscall           # printf("%s",ask1)
      li   $v0, 5
      syscall           # scanf("%d", &reg[v0])
      sw   $v0, x       # x = reg[v0]

      la   $a0, ask2    # reg[a0] = &ask2
      li   $v0, 4
      syscall           # printf("%s",ask2)
      li   $v0, 5
      syscall           # scanf("%d", &reg[v0])
      sw   $v0,  y      # y = reg[v0]

      lw  $t0, x        # reg[t0] = x
      lw  $t1, y        # reg[t1] = y
      add $a0, $t0, $t1 # reg[a0] = reg[t0] + reg[t1]
      li  $v0, 1         
      syscall           # printf("%d", reg[a0])

      la  $a0, eol
      li  $v0, 4
      syscall           # printf("%s", eol)

      li  $v0, 0	# set return value to 0
      jr  $ra           # return from main

