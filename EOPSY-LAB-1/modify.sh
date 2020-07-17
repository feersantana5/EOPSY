#!/bin/sh
#EOPSY lab 1
#Fernando Santana Falcon group 102

# the name of the script without a path
name=`basename $0`

# function for printing error messages to diagnostic output
error_msg() {
       echo "$name: error: $1" 1>&2
       exit 1
}

#function for capitalizing
upper(){
    echo "Renaming $1 with capital letters..."
  if (test -f $1)
  then
    filePath=$(dirname "${1}")
    fileName=$(basename "${1}")
    fileNameUpper=$(echo "$fileName" | tr 'a-z' 'A-Z')
    if test $fileNameUpper = $fileName
    then
      echo "File $1 was not updated"
    else
      mv "$filePath/$fileName" "$filePath/$fileNameUpper"
      if test $? = 0
      then
        echo "The file $fileNameUpper was renamed successfully"
      else
        echo "The file $fileName can not be renamed"
      fi
    fi
  else
    error_msg "The argument is not a file"
  fi
}

#function for convert to lowercase
lower(){
  echo "Renaming $1 with lowercase..."
  if (test -f $1)
  then
    filePath=$(dirname "${1}")
    fileName=$(basename "${1}")
    fileNameLower=$(echo "$fileName" | tr 'A-Z' 'a-z')
    if test $fileNameLower = $fileName
    then
      echo "File $1 was not updated"
    else
      mv "$filePath/$fileName" "$filePath/$fileNameLower"
      if test $? = 0
      then
        echo "The file $fileNameLower was renamed successfully"
      else
        echo "The file $fileName can not be renamed"
      fi
    fi
  else
    error_msg "The argument is not a file"
  fi
}

#function for use recursive access
recursive(){
  if test -f "$1"
  then
    error_msg "Files can not be renamed recursively"
  elif test -d "$1" && test "$(ls -A $1)"
  then
    for fileName in "$1"/*
    do
      if test -f "$fileName"
      then
        if test $u = "y"
        then
          upper "${fileName}"
        fi
        if test $l = "y"
        then
          lower "${fileName}"
        fi
      else
        recursive "${fileName}"
      fi
    done
  elif test -d "$1" && test !"$(ls -A $1)"
  then
    echo "The chosen folder $1 is empty"
  else
    if (test -n $1)
    then
      error_msg "The folder '$1' does not exist"
    fi
  fi
  shift
}

#function for use recursive access with sed pattern
recursiveSed()
{
  if test -z $1
  then
    break
  fi
  if (test -f "$1")
  then
    error_msg "Files can not be renamed recursively"
  elif test -d "$1" && test "$(ls -A $1)"
  then
    for fileName in "$1"/*
    do
      if test -f "$fileName"
      then
        sed_pattern "${fileName}"
      else
        recursiveSed "${fileName}"
      fi
    done
  elif test -d "$1" && test !"$(ls -A $1)"
  then
    echo "The folder $1 is empty"
  else
      error_msg "The folder '$1' does not exist"
  fi
  shift
}

#function for rename using sed pattern
sed_pattern()
{
  filePath=$(dirname "${1}")
  newfileName=$(basename "${1}"| sed "$pattern")
  if test $? != 0
  then
    error_msg "The sed pattern '$pattern' is wrong, see the sed man page"
  else
    if test "$newfileName" != "$(basename "${1}")"
    then
      echo "Renaming $1 with $pattern"
      mv "$1" "$filePath/$newfileName"
      if test $? = 0
      then
        echo "File $newfileName was ranamed successfully"
      else
        echo "File $1 can not be renamed"
      fi
    else
      echo "File $1 was not updated"
    fi
  fi
}

#function for help
help()
{
cat<<EOT

usage:
 modify [-h|--help]
 modify [-l|--lowercase] <fileName|filePath>
 modify [-s|--uppercase] <fileName|filePath>
 modify [-r|--recursive] [-l|--lowercase]|[-u|--uppercase]  <filePath>
 modify [sed_pattern] <filePath>
 modify [-r|--recursive] [sed_pattern] <filePath>

modify correct syntax examples:
 modify -u filename.txt
 modify -l filename.txt
 modify -r -l directoryName
 modify -r --uppercase directoryName
 modify --help

modify incorrect syntax examples:
 modify -d
 modify -r filename.txt
 modify

EOT

exit 1
}

#function for check all the conditions
check(){
  if (test $r = "y")
  then
    if (test $u = "y" || test $l = "y")
    then
      recursive "$arg"
    else
      if test $pattern = "n"
      then
        pattern=$arg
      else
        recursiveSed "$@"
      fi
    fi
  else
    if (test $u = "y" || test $l = "y")
    then
      if (test $u = "y")
      then
        if test -f "$arg"
        then
          upper "$arg"
        elif test -d "$arg" && test "$(ls -A $arg)"
        then
          for fileName in "$arg"/*
          do
            if test -f "$fileName"
            then
              upper "$fileName"
            fi
          done
        elif test -d "$arg" && test !"$(ls -A $arg)"
        then
          error_msg "directory $arg is empty"
        else
          error_msg "file $arg doesn't exist"
        fi
      fi
      if (test $l = "y")
      then
        if test -f "$arg"
        then
          lower "$arg"
        elif test -d $arg
        then
          for fileName in "$arg"/*
          do
            if test -f "$fileName"
            then
              lower "$fileName"
            fi
          done
        elif test -d "$arg" && test !"$(ls -A $arg)"
        then
          error_msg "The directory $arg is empty"
        else
          error_msg "The file $arg does not exist"
        fi
      fi
    else
      if test $# -le 1 && test $sed = "n"
      then
        error_msg "There are not enough args"
      fi
      if (test $sed = "y")
      then
        for fileName in "$@"
        do
          if test -f "$fileName"
          then
            echo "arg: $1"
            sed_pattern "${1}"
          elif test -d "$fileName" && test "$(ls -A $fileName)"
          then
            for fileNameIn in "$fileName"/*
            do
              if test -f "$fileNameIn"
              then
                echo "arg: $fileNameIn"
                sed_pattern "${fileNameIn}"
              fi
            done
          elif test -d "$fileName"  && test !"$(ls -A $fileName)"
          then
            echo "The directory $fileName is empty"
          else
            error_msg "The directory $fileName does not exist"
          fi
        done
      fi
      sed=y
      if test $pattern = "n"
      then
        pattern=$arg
      fi
    fi
  fi
  exe=y
}

# if no arguments given
if test -z "$1"
then
  echo "$name: There is an error, you need arguments, go to --help"
fi

# do with command line arguments
l=n
u=n
r=n
sed=n
pattern=n
exe=n
while test "x$1" != "x"
do
       case "$1" in
               -r|--recursive) r=y;;
               -l|--lowercase) l=y;;
               -u|--uppercase) u=y;;
               -h|--help) help;;
               -*) error_msg "Incorrect argument $1, go to -h"; exit 1 ;;
               *) arg="$1"; check $@;;
       esac
       shift
done

# if no enough arguments given
if (test $exe = "n")
then
  error_msg "There is not directory, filename or sed pattern in the args, go to --help"
fi
