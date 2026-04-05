# PowerShell script to create a new game object script (like Unity MonoBehaviour)
# Usage: ./create-script.ps1 -Name "Enemy"

param(
    [Parameter(Mandatory=$true)]
    [string]$Name,
    
    [Parameter(Mandatory=$false)]
    [switch]$AddToMain = $false
)

# Validate script name
if ($Name -match '[^a-zA-Z0-9_]') {
    Write-Error "Script name can only contain letters, numbers, and underscores"
    exit 1
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommandPath
$headerFile = Join-Path $scriptRoot "scripts\include\$Name.h"
$sourceFile = Join-Path $scriptRoot "scripts\src\$Name.cpp"
$mainFile = Join-Path $scriptRoot "main.cpp"

# Check if files already exist
if (Test-Path $headerFile) {
    Write-Error "Header file already exists: $headerFile"
    exit 1
}
if (Test-Path $sourceFile) {
    Write-Error "Source file already exists: $sourceFile"
    exit 1
}

# Create header file
$headerContent = @"
#ifndef $(($Name.ToUpper()))_H
#define $(($Name.ToUpper()))_H

#include "../../core/include/ObjectManager.h"

class $Name : public Object
{
public:
    $Name(float x = 0, float y = 0, float width = 50, float height = 50) 
        : Object(x, y, width, height) {}
    $Name(SDL_FRect rect) : Object(rect) {}
    $Name() : Object() {}

    void OnStart() override {
        // Initialize $Name-specific resources here
    }

    void OnUpdate() override {
        // Handle $Name logic and movement here
    }

    void OnRender(SDL_Renderer* renderer) override {
        // Render $Name here
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Blue color
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif // $(($Name.ToUpper()))_H
"@

# Create source file
$sourceContent = @"
#include "../include/$Name.h"

// Add any static member initializations or method implementations here if needed
"@

# Write files
New-Item -Path (Split-Path $headerFile) -ItemType Directory -Force | Out-Null
New-Item -Path (Split-Path $sourceFile) -ItemType Directory -Force | Out-Null

Set-Content -Path $headerFile -Value $headerContent
Set-Content -Path $sourceFile -Value $sourceContent

Write-Host "✓ Created: $headerFile" -ForegroundColor Green
Write-Host "✓ Created: $sourceFile" -ForegroundColor Green

# Optionally add to main.cpp
if ($AddToMain) {
    if (Test-Path $mainFile) {
        $mainContent = Get-Content $mainFile -Raw
        
        # Check if already included
        if ($mainContent -like "*#include `"./scripts/include/$Name.h`"*") {
            Write-Host "! Already included in main.cpp" -ForegroundColor Yellow
        } else {
            # Add include after player.h
            $mainContent = $mainContent -replace `
                '(#include "./scripts/include/player.h")', `
                "`$1`n#include `"./scripts/include/$Name.h`""
            
            Set-Content -Path $mainFile -Value $mainContent
            Write-Host "✓ Added include to main.cpp" -ForegroundColor Green
        }
        
        # Show instruction for manual instantiation
        Write-Host ""
        Write-Host "Next step: Add this to main.cpp (in the game loop setup):" -ForegroundColor Cyan
        Write-Host "$Name* $($Name.ToLower()) = new $Name(100, 100, 50, 50);" -ForegroundColor Yellow
    }
}

Write-Host ""
Write-Host "Script created successfully! " -ForegroundColor Green
Write-Host "Usage in main.cpp: $Name* $($Name.ToLower()) = new $Name(x, y, width, height);" -ForegroundColor Cyan
