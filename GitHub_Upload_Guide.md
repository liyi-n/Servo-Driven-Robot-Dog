# STM32项目上传GitHub详细指南

## 1. 安装Git

### Windows平台安装步骤：
1. 访问Git官方网站：[https://git-scm.com/](https://git-scm.com/)
2. 点击"Download for Windows"下载最新版本
3. 运行安装程序，按照默认选项安装即可（建议勾选"Add Git to PATH"选项）
4. 安装完成后，打开命令提示符或PowerShell，输入`git --version`验证安装成功

## 2. 创建GitHub仓库

1. 访问GitHub官网：[https://github.com/](https://github.com/)
2. 登录您的GitHub账号
3. 点击右上角的"+"图标，选择"New repository"
4. 填写仓库信息：
   - Repository name: 输入仓库名称（例如：ADOG-STM32-Project）
   - Description: 可选，填写项目描述
   - Public/Private: 选择仓库类型
   - Initialize this repository with: 不要勾选任何选项
5. 点击"Create repository"

## 3. 配置Git并上传项目

### 步骤1：配置Git用户信息
打开命令提示符或PowerShell，执行以下命令：

```bash
git config --global user.name "您的GitHub用户名"
git config --global user.email "您的GitHub邮箱"
```

### 步骤2：进入项目目录

```bash
cd d:\Users\ly\STM32CubeIDE\workspace_1.19.0\ADOG
```

### 步骤3：初始化本地仓库

```bash
git init
```

### 步骤4：添加所有文件到暂存区

```bash
git add .
```

### 步骤5：提交代码

```bash
git commit -m "Initial commit"
```

### 步骤6：添加远程仓库

将`<your-username>`和`<your-repository-name>`替换为您的GitHub用户名和仓库名称：

```bash
git remote add origin https://github.com/<your-username>/<your-repository-name>.git
```

### 步骤7：推送到GitHub

```bash
git push -u origin main
```

## 4. 注意事项

1. 首次推送时需要输入GitHub账号和密码进行身份验证
2. 如果您的GitHub使用了SSH密钥，可以使用SSH地址代替HTTPS地址
3. 如果出现"fatal: remote origin already exists"错误，可以先执行`git remote remove origin`，然后重新添加远程仓库
4. 如果出现分支名称不匹配错误（例如：main vs master），可以使用以下命令重命名本地分支：
   ```bash
   git branch -M main
   ```

## 5. 查看上传结果

上传完成后，访问您的GitHub仓库页面，即可看到已上传的项目代码。

## 6. 后续更新代码

每次修改代码后，执行以下命令更新GitHub仓库：

```bash
git add .
git commit -m "更新描述"
git push
```

---

如果您在执行过程中遇到任何问题，请参考GitHub官方文档或搜索相关错误信息。