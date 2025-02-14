# Source global definitions
if test -f /etc/bashrc
    source /etc/bashrc
end

# Get the current working directory
set workdir (pwd)

# Get the directory of the current script
set script_dir (dirname (status --current-filename))
set script_dir (realpath $script_dir)
set PROJECT_DIR $script_dir

# Check if the BluetAna subdirectory exists within PROJECT_DIR
if test -d "$PROJECT_DIR/BluetAna"
    echo "===> PROJECT_DIR is set to: $PROJECT_DIR"
else
    echo "***: The BluetAna subdirectory does not exist within PROJECT_DIR."
end

# bluet
set -x BLUETBase $PROJECT_DIR
set -x BLUETCONFIG $PROJECT_DIR/BluetConfig/BluetConfigFile_qyj.cfg
# root
set -x ROOT_LIBRARIES (root-config --libs)

# alias
abbr duc 'du -h --max-depth=1'
abbr la 'ls -all'

# ulimit set
ulimit -d unlimited
ulimit -s unlimited
ulimit -m unlimited
ulimit -v unlimited

# Run Python script
python $PROJECT_DIR/Scripts/dependence/Json_Start.py

# Return to the original working directory
cd $workdir



