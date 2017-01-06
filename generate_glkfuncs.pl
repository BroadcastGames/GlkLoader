#!/usr/bin/perl
# generate_glkfuncs.pl glk.h func.txt

# (C)opyright 1999 by John Cater <katre@ruf.rice.edu>, 2001 by Joe Mason
# <jcmason@uwaterloo.ca>
# Much help received from the greal guys at ifMUD 
# (http://ifmud.port4000.org:4001), especially Dan Shiovitz.

# A perl program to generate C functions to dynically load the symbols for the
# corresponding to glk functions.  The arguments are a glk.h file and a 
# func.txt file.  glk.h can be obtained from your glk library, or from 
# http://www.eblong.com/zarf/glk.  func.txt is a list of the glk funcs 
# accompanied by their selector numbers.  This list can be built from the glk
# specifiation file, at the above address, but should be distributed with this
# file.
#
# Adapted by Joe Mason from glk2inf, a similar program to generate Inform
# functions for the corresponding glk functions, written by John Cater.

sub argWithStar
{
   $_[0];
}

sub argWithoutStar
{
   @chars = split (//, $_[0]);
   if ($chars[0] eq "*")
   {
      shift @chars;
   }
   join ("", @chars); 
}

sub createArgList
{
   my @args = ();
   foreach $i (@_)
   { 
      if ($i->[0] eq "void")
      {
         if (defined($i->[1]))
         {
            # This is a function pointer
            $signature = $i->[0];
            $signature .= "(";
            $signature .= argWithStar($i->[1]);
            $signature .= ")(";
            $signature .= $i->[2];
            $signature .= ")";
            push (@args, $signature);
         }
         else
         {
            # This is a void parameter
            $signature = $i->[0];
            push (@args, $signature);
         }
      }
      else
      {
         # This is a regular parameter
         $signature = $i->[0];
         $signature .= " ";
         $signature .= argWithStar($i->[1]);
         push (@args, $signature);
      }
   }
   join (", ", @args);
}

sub createVarList
{
   my @args = ();
   foreach $i (@_)
   {
      push (@args, argWithoutStar($i->[1]));
   }
   join (", ", @args);
}

if ($#ARGV != 1)
{
   die "Usage: generate_glkfuncs.pl glk.h func.txt";
}

$glk_file = $ARGV[0];
$func_file = $ARGV[1];

# First, read in func.txt
open (FUNC_FILE, "<$func_file") ||
   die "Can't open func.txt";

$num_funcs = 0;
$func_data = {};
$func_hash = {};

while (defined ($line = <FUNC_FILE>))
{
   #the format is 'number: func'
   if ($line =~ /\s*(0x[0-9A-Fa-f]+):\s+(\w+)/)
   {
      $number = hex($1); $name = $2;
      $func_data[$num_funcs]->{name} = $name;
      $func_data[$num_funcs]->{selector} = $number;
      $func_hash->{$name} = $num_funcs;
      $num_funcs++;
   }
}
close (FUNC_FILE);

print "Read in $num_funcs function selectors.\n";

# Next, read in glk.h (headache)
open (GLK_FILE, "<$glk_file") ||
   die "Can't open glk.h.";

$num_glk_cons = 0;
$glk_cons_data = {};
$glk_cons_hash = {};

$num_glk_funcs = 0;
$glk_func_data = {};
$glk_func_hash = {};

while (defined ($line = <GLK_FILE>))
{
   #First, check for constant #def's.
   # The format is '#define name (val)'
   if ($line =~ /#define\s+(\w+)\s+\((?:0x([0-9A-Fa-f]+)|(\d+))\)/)
   {
      $name = $1;
      if (defined($2)) # hex number
      {
         $value = "\$" . $2;
      } else # decimal number
      {
         $value = $3;
      }
      $glk_cons_data[$num_glk_cons]->{name} = $name;
      $glk_cons_data[$num_glk_cons]->{value} = $value;
      $glk_cons_hash->{$name} = $num_glk_cons;
      $num_glk_cons++;
   }

   #Second, check for functions
   # The format is 'extern ret_type name(arg_type args, ...);'
   if ($line =~ /^extern/)
   {
      my @arg_list = ();

      # See if this line is complete
      chomp $line;
      if ($line !~ /;/) # it's not over yet
      {
         #get the next line, and check it
         #no function declaration is longer than 2 lines
         $next = <GLK_FILE>;
         chomp $next;
         if ($next !~ /;/) # This is a problem
         {
            print "Warning: Function not complete.\n";
            next;
         }
         # concatenate the two lines
         $line = $line . $next;
      } # otherwise, the line is fine

#print "Before: [$line]\n";
      @func = split(/[^a-zA-Z0-9_\*]+/,$line);
#print "After: [", join(",", @func), "]\n";
#print "\n";

      $ret_type = shift @func;
      if ($ret_type eq "extern")
      {
         $ret_type = shift @func;
      }
      if ($ret_type eq "unsigned")
      {
         $ret_type .= " ";
         $ret_type .= shift @func;
      }

      $name = shift @func;

      $num_args = 0;
      $type = "";
      while (defined($arg = shift @func))
      {
         $type .= $arg;

         if ($arg eq "unsigned")
         {
            $type .= " ";
            next;
         }

         if ($arg eq "void")
         {
            # check for another entry, to see if it's a func pointer
            if (@func != 0)
            {
               $arg = shift @func;
               $suffix = shift @func;
               $arg_list[$num_args] = [ $type, $arg, $suffix ];
            } else
            {
               $arg_list[$num_args] = [ "void" ];
            }
         } else
         {
            $arg = shift @func;
            $arg_list[$num_args] = [ $type, $arg ];
            $type = "";
         }

         $num_args++;
      }

      if ($name eq "glk_main")
      {
         next;
      }

      $glk_func_data[$num_glk_funcs]->{name} = $name;
      $glk_func_data[$num_glk_funcs]->{ret_type} = $ret_type;
      $glk_func_data[$num_glk_funcs]->{num_args} = $num_args;
      $glk_func_data[$num_glk_funcs]->{arg_list} = \@arg_list;
      $glk_func_data[$num_glk_funcs]->{arg_type_list} = \@arg_type_list;

      $glk_func_hash->{$name} = $num_glk_funcs;
      $num_glk_funcs++;
   }
   
}
close (GLK_FILE);

print "Read in $num_glk_cons Glk Constants.\n";
print "Read in $num_glk_funcs Glk Functions.\n";

#Okay, input is done.  Time to start writing.
open (OUT_FILE, ">glkfuncs.c") ||
   die "Can't open glkfuncs.c";

# Write in things like the filename, copyright info, etc.
($sec,$min,$hour,$mday,$month,$year,$wday,$yday,$isdst) = localtime(time);

$date = sprintf("%02d%02d%02d at %02d:%02d.%02d",
                 $year, $month + 1, $mday, $hour, $min, $sec);

print OUT_FILE "/**\n";
print OUT_FILE " * glkfuncs.c\n";
print OUT_FILE " * Dynamically created by generate_glkfuncs.pl on $date.\n";
print OUT_FILE " *\n";
print OUT_FILE " * Copyright (c) 2001 Joe Mason <jcmason\@uwaterloo.ca>\n";
print OUT_FILE " * All rights reserved.\n";
print OUT_FILE " *\n";
print OUT_FILE " * Redistribution and use in source and binary forms, with or without\n";
print OUT_FILE " * modification, are permitted provided that the following conditions\n";
print OUT_FILE " * are met:\n";
print OUT_FILE " * 1. Redistributions of source code must retain the above copyright\n";
print OUT_FILE " *    notice, this list of conditions and the following disclaimer.\n";
print OUT_FILE " * 2. Redistributions in binary form must reproduce the above copyright\n";
print OUT_FILE " *    notice, this list of conditions and the following disclaimer in the\n";
print OUT_FILE " *    documentation and/or other materials provided with the distribution.\n";
print OUT_FILE " * 3. The name of the author may not be used to endorse or promote products\n";
print OUT_FILE " *    derived from this software without specific prior written permission.\n";
print OUT_FILE " *\n";
print OUT_FILE " *    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR\n";
print OUT_FILE " *    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n";;
print OUT_FILE " *    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n";
print OUT_FILE " *    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,\n";
print OUT_FILE " *    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT\n";
print OUT_FILE " *    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n";
print OUT_FILE " *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\n";
print OUT_FILE " *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n";
print OUT_FILE " *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF\n";
print OUT_FILE " *    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";
print OUT_FILE " */\n";
print OUT_FILE "\n";

# Global definitions 
print OUT_FILE "#include <stdio.h>\n";
print OUT_FILE "#include <glk.h>\n";
print OUT_FILE "\n";
print OUT_FILE "#include \"defines.h\"\n";
print OUT_FILE "\n";
print OUT_FILE "extern LIB_HANDLE gGlkLib;\n";
print OUT_FILE "\n";

# Now, the functions
$num_funcs = 0;
for ($i = 0; $i < $num_glk_funcs; $i++)
{
   my $name = $glk_func_data[$i]->{name};
   my $ret_type = $glk_func_data[$i]->{ret_type};
   my $num_args = $glk_func_data[$i]->{num_args};
   my @arg_list = @{$glk_func_data[$i]->{arg_list}};
   if (!defined ($func_data[$func_hash->{$name}]->{selector}))
   {
      # This function does not have a selector.  Make a note, and skip it.
      print "Glk Function $name does not have a selector.\n";
      next;
   }
   my $selector = $func_data[$func_hash->{$name}]->{selector};

   print OUT_FILE "$ret_type $name(", createArgList (@arg_list), ")\n";
   print OUT_FILE "{\n";
   print OUT_FILE "    $ret_type (*funcPtr)(", createArgList (@arg_list), ");\n";
   print OUT_FILE "    char * errBuf = NULL;\n";
   print OUT_FILE "\n";
   print OUT_FILE "    funcPtr = loadSymbol(gGlkLib, \"$name\", &errBuf);\n";
   print OUT_FILE "    if (NULL != errBuf) {\n";
   print OUT_FILE "        fprintf(stderr, \"%s\\n\", errBuf);\n";
   print OUT_FILE "        exit(1);\n";
   print OUT_FILE "    }\n";
   print OUT_FILE "\n";
   if ($ret_type eq "void")
   {
      print OUT_FILE "    ";
   }
   else
   {
      print OUT_FILE "    return ";
   }
   print OUT_FILE "(*funcPtr)(", createVarList (@arg_list), ");\n";
   print OUT_FILE "}\n";
   print OUT_FILE "\n";

   $num_funcs++;
}
print "Processed $num_funcs functions.\n";
