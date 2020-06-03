PATHPLOTS=$1
NEWFOLDER=$2
if [ -d $PATHPLOTS ]
then
  echo $PATHPLOTS "exists"
else
  echo $PATHPLOTS "does not exists. Break."
  break
fi

if [ -d $NEWFOLDER ]
then
  echo $NEWFOLDER "exists"
else
  echo $NEWFOLDER "does not exists. Create."
  mkdir $NEWFOLDER
fi

cp -r $PATHPLOTS/* $NEWFOLDER
echo "All plots were copied in "$NEWFOLDER

