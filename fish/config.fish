if status is-interactive
    # Commands to run in interactive sessions can go here
end

#auto i3 
if [ (tty) = "/dev/tty1" ]
	cd ~
	startx
end

set -Ux PATH $HOME/.pyenv/bin $PATH
status --is-interactive; and source (pyenv init -|psub)


#GTK3
#scale
#set -Ux GDK_SCALE 1.75
#set -Ux GDK_DPI_SCALE 0.5
#
#QT5
#scale
#set -Ux QT_SCALE_FACTOR 1.75
#vscode
