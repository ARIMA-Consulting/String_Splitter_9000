# GitHub Setup Guide

This guide will help you connect your local repository to GitHub and set up easy syncing.

## Step 1: Create a GitHub Repository

1. Go to [GitHub.com](https://github.com) and sign in
2. Click the "+" icon in the top right, then "New repository"
3. Name it (e.g., `string-splitter-9000`)
4. **Do NOT** initialize with README, .gitignore, or license (we already have these)
5. Click "Create repository"

## Step 2: Add GitHub Remote

After creating the repository, GitHub will show you commands. Choose one:

### Option A: HTTPS (Easier, requires authentication)
```bash
cd /Users/tylerharris/Desktop/learnc/String_Splitter_9000
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
```

### Option B: SSH (More secure, requires SSH key setup)
```bash
cd /Users/tylerharris/Desktop/learnc/String_Splitter_9000
git remote add origin git@github.com:YOUR_USERNAME/YOUR_REPO_NAME.git
```

**Replace:**
- `YOUR_USERNAME` with your GitHub username
- `YOUR_REPO_NAME` with your repository name

## Step 3: Verify Remote

Check that the remote was added correctly:
```bash
git remote -v
```

You should see:
```
origin  https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git (fetch)
origin  https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git (push)
```

## Step 4: Push Your Code

### First Push (sets upstream branch)
```bash
git push -u origin main
```

After this, you can use the sync script for future pushes.

## Step 5: Use the Sync Script

Make the script executable (if not already):
```bash
chmod +x sync_github.sh
```

Then sync with GitHub:
```bash
./sync_github.sh
```

The script will:
- Check for uncommitted changes
- Fetch latest changes from GitHub
- Show you what will be pushed
- Push your commits to GitHub

## Authentication

### HTTPS Authentication

If using HTTPS, you'll need to authenticate. GitHub no longer accepts passwords, so you need:

1. **Personal Access Token (PAT)**
   - Go to GitHub Settings → Developer settings → Personal access tokens → Tokens (classic)
   - Generate a new token with `repo` scope
   - Use this token as your password when pushing

2. **Git Credential Helper** (recommended)
   ```bash
   git config --global credential.helper osxkeychain  # macOS
   ```
   This stores your credentials securely.

### SSH Authentication

If using SSH, you need to set up an SSH key:

1. **Check for existing SSH key:**
   ```bash
   ls -al ~/.ssh
   ```

2. **Generate new SSH key (if needed):**
   ```bash
   ssh-keygen -t ed25519 -C "your_email@example.com"
   ```

3. **Add SSH key to GitHub:**
   ```bash
   cat ~/.ssh/id_ed25519.pub
   ```
   Copy the output and add it to GitHub: Settings → SSH and GPG keys → New SSH key

4. **Test connection:**
   ```bash
   ssh -T git@github.com
   ```

## Quick Sync Command

After setup, you can sync with a single command:
```bash
./sync_github.sh
```

Or create an alias in your `~/.zshrc` or `~/.bashrc`:
```bash
alias sync-github='cd /Users/tylerharris/Desktop/learnc/String_Splitter_9000 && ./sync_github.sh'
```

Then just run `sync-github` from anywhere!

## Troubleshooting

### "Remote already exists"
If you get this error, you can update the remote URL:
```bash
git remote set-url origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
```

### "Permission denied"
- Check your authentication (PAT for HTTPS, SSH key for SSH)
- Verify you have push access to the repository

### "Branch is behind"
The sync script will ask if you want to pull first. Say yes to merge remote changes.

### "Nothing to push"
This means your local and remote are already in sync!

## Daily Workflow

1. Make changes to your code
2. Commit changes: `./commit.sh "Your message"`
3. Sync to GitHub: `./sync_github.sh`

That's it! Your code is now backed up and synced with GitHub.

