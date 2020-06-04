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
  echo $NEWFOLDER "does not exists. Create a link to "$PATHPLOTS
  ln -s $PATHPLOTS .
fi

#cp -r $PATHPLOTS/* $NEWFOLDER
echo "All plots were linked in "$NEWFOLDER

