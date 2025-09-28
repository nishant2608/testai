# Ensure the script runs from its own directory
cd $PSScriptRoot

# Create virtual environment
python -m venv .venv

# Activate virtual environment
. .\.venv\Scripts\Activate.ps1

# Install dependencies
pip install -r requirements.txt

# Run the Python file
python mcp_srv_src.py

echo "Script execution completed."