-- Set the file encoding format to UTF-8 uniformly.
vim.g.encoding = "UTF-8"
vim.o.fileencoding = "UTF-8"

-- Set the number of spaces used for each indentation level.
vim.opt.shiftwidth = 2
vim.opt.tabstop = 2

-- Enable automatic indentation.
vim.opt.ai = true

-- Enable smart indentation.
vim.opt.si = true

-- Neovim will automatically add spaces or tabs on new lines to match the existing indentation level.
vim.opt.autoindent = true
-- Display line numbers.
vim.wo.number = true

-- Highlight the current line where the cursor is located.
vim.wo.cursorline = true

-- Display the left icon indicator column.
vim.wo.signcolumn = "yes"

-- Display the right reference line.
--vim.wo.colorcolumn = "80"

-- Set the command line height to 2 to provide enough display space.
vim.o.cmdheight = 2

-- The movement length when using >> <<.
vim.o.shiftwidth = 2
vim.bo.shiftwidth = 2

-- When the cursor is at the beginning or end of a line, <Left> and <Right> can jump to the next line, which is very useful.
vim.o.whichwrap = '<,>,[,]'

-- Enhanced completion.
vim.o.wildmenu = true

-- Display of invisible characters, here only spaces are displayed as dots.
-- vim.o.list = true
-- vim.o.listchars = "space:·"

-- Always show the tabline, which will display the file name at the top of the left status column.
vim.o.showtabline = 2
-- Highlight all matches of the search pattern.
vim.opt.hlsearch = true

-- Search is case-insensitive unless it contains uppercase letters.
vim.o.ignorecase = true
vim.o.smartcase = true
-- Automatically load the file when it is modified by an external program.
vim.o.autoread = true
vim.bo.autoread = true
-- Disable netrw at the very beginning of your init.lua.
vim.g.loaded_netrw = 1
vim.g.loaded_netrwPlugin = 1

-- Set termguicolors to enable highlight groups.
vim.opt.termguicolors = true

