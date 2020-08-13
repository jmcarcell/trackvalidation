#source ILCSoft
function source_ILCSoft {
  echo ">>> Check status of \$MARLIN_DLL"
  if [ -z "$MARLIN_DLL" ]
  then
      echo "\$MARLIN_DLL is empty"
  else
      echo "\$MARLIN_DLL is NOT empty, clean up"
      unset MARLIN_DLL
  fi
  echo ">>> Source ILCSoft release $1"
  source /cvmfs/clicdp.cern.ch/iLCSoft/builds/$1/x86_64-slc6-gcc62-opt/init_ilcsoft.sh
  echo ">>> Marlin library:"
  echo $MARLIN_DLL
}

