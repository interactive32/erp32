<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:50
         compiled from ./atk/themes/default/templates/dgpaginator.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'escape', './atk/themes/default/templates/dgpaginator.tpl', 9, false),)), $this); ?>
<?php $_from = $this->_tpl_vars['links']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['i'] => $this->_tpl_vars['link']):
?>
  <?php if ($this->_tpl_vars['i'] > 0): ?>
    |
  <?php endif; ?>
  
  <?php if ($this->_tpl_vars['link']['current']): ?>
    <b><?php echo $this->_tpl_vars['link']['title']; ?>
</b>
  <?php else: ?>
    <a href="javascript:void(0)" onclick="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['call'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
" title="<?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
"><?php echo ((is_array($_tmp=$this->_tpl_vars['link']['title'])) ? $this->_run_mod_handler('escape', true, $_tmp) : smarty_modifier_escape($_tmp)); ?>
</a>
  <?php endif;  endforeach; endif; unset($_from); ?>