function get_resolution {
	xrandr | egrep current | perl -n -e 'm/current ([0-9]+) x ([0-9]+)/ ; print "$1 $2"'
}


