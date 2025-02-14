-- Define the installation path for lazy.nvim
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
-- Check if lazy.nvim is not installed
if not vim.loop.fs_stat(lazypath) then
  -- If not installed, clone the lazy.nvim repository using git
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable", -- Use the latest stable release
    lazypath,
  })
end

-- Prepend the lazy.nvim path to Neovim's runtime path
vim.opt.rtp:prepend(lazypath)


-- Initialize lazy.nvim and set up plugins
require("lazy").setup({
  -- All plugin installations are configured here
  -- Color scheme plugin. The choice depends on personal preference.
  {
    "folke/tokyonight.nvim",
    lazy = false, -- Load this plugin immediately on startup
    priority = 1000, -- Set a high priority for this plugin
    opts = {} -- Use default options for this plugin
  },
  -- File manager plugin
  {
    "nvim-tree/nvim-tree.lua",
    version = "*", -- Use the latest version of the plugin
    lazy = false, -- Load this plugin immediately on startup
    dependencies = {"nvim-tree/nvim-web-devicons"}, -- This plugin depends on nvim-web-devicons
    config = function()
      -- Configure the nvim-tree plugin with default settings
      require("nvim-tree").setup {}
    end
  },
  -- Syntax highlighting plugin that supports multiple languages
  {"nvim-treesitter/nvim-treesitter"},
  -- File status display plugin
  {
    'nvim-lualine/lualine.nvim',
    config = function()
      -- Configure the lualine plugin with default settings
      require('lualine').setup()
    end
  },
  -- nvim-cmp for code completion
  {
        "hrsh7th/nvim-cmp",
        event = "InsertEnter",
        dependencies = {
            -- Completion source from LSP
            "hrsh7th/cmp-nvim-lsp",
            -- Completion source from buffer
            "hrsh7th/cmp-buffer",
            -- Completion source from path
            "hrsh7th/cmp-path",
            -- Snippet engine
            "L3MON4D3/LuaSnip",
            -- Integrate LuaSnip with nvim-cmp
            "saadparwaiz1/cmp_luasnip",
        },
        config = function()
            local cmp = require("cmp")
            local luasnip = require("luasnip")

            cmp.setup({
                snippet = {
                    expand = function(args)
                        luasnip.lsp_expand(args.body)
                    end,
                },
                mapping = cmp.mapping.preset.insert({
                    ["<C-b>"] = cmp.mapping.scroll_docs(-4),
                    ["<C-f>"] = cmp.mapping.scroll_docs(4),
                    ["<C-Space>"] = cmp.mapping.complete(),
                    ["<C-e>"] = cmp.mapping.abort(),
                    ["<CR>"] = cmp.mapping.confirm({ select = true }),
                }),
                sources = cmp.config.sources({
                    { name = "nvim_lsp" },
                    { name = "luasnip" },
                }, {
                    { name = "buffer" },
                }),
            })
        end,
    },
    -- nvim-lspconfig for LSP configuration
    {
        "neovim/nvim-lspconfig",
        dependencies = {
            -- Mason for LSP server installation
            "williamboman/mason.nvim",
            "williamboman/mason-lspconfig.nvim",
        },
        config = function()
            require("mason").setup()
            require("mason-lspconfig").setup({
                ensure_installed = {
                    -- Language servers for different languages
                    "jsonls", -- For JSON
                    "pyright", -- For Python
                    "lemminx", -- For XML
                    "bashls", -- For Shell
                    "clangd", -- For C and C++
										"cmake", -- CMake LSP servers
                },
            })

            local lspconfig = require("lspconfig")
            local capabilities = require("cmp_nvim_lsp").default_capabilities()

            -- Configure each language server
            local servers = { "jsonls", "pyright", "lemminx", "bashls", "clangd","cmake" }
            for _, server in ipairs(servers) do
                lspconfig[server].setup({
                    capabilities = capabilities,
                })
            end
        end,
    }
})








