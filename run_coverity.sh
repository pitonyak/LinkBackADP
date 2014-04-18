rm LinkBackADP.tgz
cov-build --dir cov-int make Makefile.Debug all
tar czvf LinkBackADP.tgz cov-int
