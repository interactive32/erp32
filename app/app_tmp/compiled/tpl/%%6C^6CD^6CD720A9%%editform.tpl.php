<?php /* Smarty version 2.6.11, created on 2013-01-18 01:21:10
         compiled from ./atk/themes/steelblue/templates/editform.tpl */ ?>
      <tr>
        <td valign="top">
          <table>
  <?php $_from = $this->_tpl_vars['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <?php if ($this->_tpl_vars['field']['column'] != 1):  $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "theme:field.tpl", 'smarty_include_vars' => array('field' => $this->_tpl_vars['field'])));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
  endif; ?>
  <?php endforeach; endif; unset($_from); ?>
          </table>
        </td>
        <td valign="top">
          <table>
  <?php $_from = $this->_tpl_vars['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <?php if ($this->_tpl_vars['field']['column'] == 1):  $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "theme:field.tpl", 'smarty_include_vars' => array('field' => $this->_tpl_vars['field'])));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
  endif; ?>
  <?php endforeach; endif; unset($_from); ?>
          </table>
        </td>
      </tr>  