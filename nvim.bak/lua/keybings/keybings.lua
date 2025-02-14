-- leader_key
vim.g.mapleader = " "
vim.g.maplocalleader = " "

local opt = { noremap = true, silent = true }
local map = vim.api.nvim_set_keymap

-- alt + m open or close file browser Nvim-Tree  
map("n", "<A-m>", ":NvimTreeToggle<CR>", opt)

