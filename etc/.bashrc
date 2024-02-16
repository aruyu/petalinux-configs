# ~/.bashrc: executed by bash(1) for non-login shells.

if [[ $EUID -eq 0 ]]; then
  PS1='[\u@\h \W]\$ '
else
  PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
fi

umask 022

# You may uncomment the following lines if you want `ls' to be colorized:
export LS_OPTIONS='--color=auto'
# eval `dircolors`
alias ls='ls $LS_OPTIONS'
alias ll='ls $LS_OPTIONS -lahF'
alias lt='ls $LS_OPTIONS -lahFt'
alias la='ls $LS_OPTIONS -A'
alias l='ls $LS_OPTIONS -CF'
alias grep='grep $LS_OPTIONS'
alias fgrep='fgrep $LS_OPTIONS'
alias egrep='egrep $LS_OPTIONS'

# Some more alias to avoid making mistakes:
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'
