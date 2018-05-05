#! /bin/bash
# bash wrapper to generate plots with GNU Plot
# Example Usage: ./plot.sh ../logs/2018_05_02_06-46-42_session.csv

if [[ -z $1 ]]; then
  echo "you forgot to specify an input file... exit."
  exit 
fi

# Set up variables used in plotting. 
HEART_RATE_OUT=`echo $1|sed 's/session.csv/heartrate.svg/'|sed 's|../logs/||'`
SPEED_INCLINE_OUT=`echo $1|sed 's/session.csv/speed_incline.svg/'|sed 's|../logs/||'`
LOG_AS_TEXT=`echo $1|sed 's/session.csv/session.csv/'|sed 's|../logs/||'`
HTML_OUT=`echo $1|sed 's/session.csv/session.html/'|sed 's|../logs/||'`
TITLE_VALUE=`echo $1|sed 's/_session.csv//'| sed 's|../logs/||'`

echo "input was:         $1"
echo "HEART_RATE_OUT:    $HEART_RATE_OUT"
echo "SPEED_INCLINE_OUT: $SPEED_INCLINE_OUT"
echo "HTML_OUT:          $HTML_OUT"
echo "TITLE_VALUE:       $TITLE_VALUE"

# Copy data file to plots directory
cp $1 ../plots/

# Perform the Plotting.
  gnuplot -e "session_csv='$1'; heart_rate_out='../plots/$HEART_RATE_OUT'; speed_incline_out='../plots/$SPEED_INCLINE_OUT'" -c ./plot_session.gnup 
   
# generate HTML template for previewing plots.
  cp ./template.html ../plots/$HTML_OUT
  sed -i "s/csv_url/$LOG_AS_TEXT/ g" ../plots/$HTML_OUT
  sed -i "s/title_value/$TITLE_VALUE/ g" ../plots/$HTML_OUT
   
  sed -i "s/heartrate_image/$HEART_RATE_OUT/ g" ../plots/$HTML_OUT
  sed -i "s/speed_incline_image/$SPEED_INCLINE_OUT/ g" ../plots/$HTML_OUT
