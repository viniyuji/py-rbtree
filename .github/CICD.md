# GitHub Actions CI/CD Setup

This repository uses GitHub Actions for automated testing and deployment to PyPI.

## Workflows

### 1. Build and Test (CI)
**File**: `.github/workflows/ci.yml`

Runs on every push and pull request to test the package across:
- **Operating Systems**: Ubuntu, macOS, Windows
- **Python Versions**: 3.7, 3.8, 3.9, 3.10, 3.11, 3.12

### 2. Publish to PyPI (CD)
**File**: `.github/workflows/publish.yml`

Automatically publishes to PyPI when you create a GitHub release.

## Setup Instructions

### Step 1: Configure PyPI Trusted Publishing

Trusted publishing is the modern, secure way to publish to PyPI without using passwords or API tokens.

1. Go to [PyPI](https://pypi.org) and log in
2. Navigate to your account settings
3. Click on "Publishing" tab
4. Add a new pending publisher:
   - **PyPI Project Name**: `rbtree`
   - **Owner**: Your GitHub username (e.g., `yourusername`)
   - **Repository**: Your repo name (e.g., `python-rbtree`)
   - **Workflow name**: `publish.yml`
   - **Environment name**: `pypi`

5. Repeat for TestPyPI at [test.pypi.org](https://test.pypi.org):
   - Same settings but environment name: `testpypi`

### Step 2: Configure GitHub Environments

1. Go to your GitHub repository settings
2. Click on "Environments"
3. Create two environments:
   - `pypi` (for production)
   - `testpypi` (for testing)

4. (Optional) Add protection rules:
   - Require reviewers before deployment
   - Restrict to specific branches

## How to Release

### Option 1: Create a GitHub Release (Recommended)

1. Update version in `setup.py` and `pyproject.toml`
2. Commit and push:
   ```bash
   git add setup.py pyproject.toml
   git commit -m "Bump version to 1.0.1"
   git push
   ```

3. Create and push a tag:
   ```bash
   git tag v1.0.1
   git push origin v1.0.1
   ```

4. Go to GitHub → Releases → "Create a new release"
5. Select your tag (v1.0.1)
6. Write release notes
7. Click "Publish release"

The workflow will automatically:
- Build the package
- Run tests
- Publish to PyPI

### Option 2: Manual Trigger (TestPyPI)

For testing, you can manually trigger the workflow:

1. Go to Actions tab
2. Select "Publish to PyPI"
3. Click "Run workflow"
4. Select branch
5. Click "Run workflow"

This will publish to **TestPyPI** only.

## Monitoring Deployments

- **Actions Tab**: View workflow runs and logs
- **Environments**: See deployment history and status
- **PyPI**: Check package page at https://pypi.org/project/rbtree/

## Troubleshooting

**Issue**: Workflow fails with "403 Forbidden"  
**Solution**: Check that trusted publishing is configured correctly on PyPI

**Issue**: "Environment protection rules not satisfied"  
**Solution**: Add required reviewers or remove protection rules from environment

**Issue**: Build fails on specific Python version/OS  
**Solution**: Check the CI logs to identify platform-specific issues

## Local Testing Before Release

Before creating a release, test locally:

```bash
make clean
make test           # Run tests
make dist           # Build distribution
twine check dist/*  # Verify distribution
```

## Alternative: Using API Tokens (Legacy)

If you prefer API tokens instead of trusted publishing:

1. Create API token on PyPI
2. Add to GitHub Secrets:
   - Name: `PYPI_API_TOKEN`
   - Value: Your token
3. Modify `publish.yml` to use token instead of trusted publishing

See `PUBLISHING.md` for manual publishing instructions.
