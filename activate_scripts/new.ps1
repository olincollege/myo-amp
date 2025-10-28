$command1 = "python -m venv .venv"
$command2 = "./.venv/Scripts/Activate.ps1"
$command3 = "pip install -r requirements.txt"

Invoke-Expression $command1
Invoke-Expression $command2
Invoke-Expression $command3