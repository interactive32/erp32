<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:50
         compiled from ./atk/themes/default/templates/datagrid.tpl */ ?>
<table border="0" cellspacing="0" cellpadding="2">
<?php if ($this->_tpl_vars['top']): ?>
<tr>
  <td align="left" valign="top" colspan="2">
    <?php echo $this->_tpl_vars['top']; ?>

  </td>
</tr>
<?php endif;  if ($this->_tpl_vars['index'] || $this->_tpl_vars['editcontrol']): ?>
  <tr>
    <td align="left" valign="top">
      <?php if ($this->_tpl_vars['editcontrol']):  echo $this->_tpl_vars['editcontrol'];  endif; ?> <?php if ($this->_tpl_vars['index']):  echo $this->_tpl_vars['index'];  endif; ?>
    </td>
  </tr>
<?php elseif ($this->_tpl_vars['paginator'] || $this->_tpl_vars['limit']): ?>
  <tr>
    <td align="left" valign="middle">
      <?php if ($this->_tpl_vars['editcontrol']):  echo $this->_tpl_vars['editcontrol'];  endif; ?> <?php if ($this->_tpl_vars['paginator']):  echo $this->_tpl_vars['paginator'];  endif; ?>
    </td>
    <td align="right" valign="middle">
      <?php if ($this->_tpl_vars['limit']):  echo $this->_tpl_vars['limit'];  endif; ?>
    </td>
  </tr>
<?php endif;  if ($this->_tpl_vars['list']): ?>
<tr>
  <td align="left" valign="top" colspan="2">
    <?php echo $this->_tpl_vars['list']; ?>

  </td>
</tr>
<?php endif;  if ($this->_tpl_vars['norecordsfound']): ?>
  <tr>
    <td align="left" valign="top">
      <i><?php echo $this->_tpl_vars['norecordsfound']; ?>
</i>
    </td>
  </tr>
<?php endif;  if ($this->_tpl_vars['paginator'] || $this->_tpl_vars['summary']): ?>
  <tr>
    <td align="left" valign="middle">
      <?php if ($this->_tpl_vars['paginator']):  echo $this->_tpl_vars['paginator'];  endif; ?>
    </td>
    <td align="right" valign="middle">
      <?php if ($this->_tpl_vars['summary']):  echo $this->_tpl_vars['summary'];  endif; ?>
    </td>
  </tr>
<?php endif;  if ($this->_tpl_vars['bottom']): ?>
<tr>
  <td align="left" valign="top" colspan="2">
    <?php echo $this->_tpl_vars['bottom']; ?>

  </td>
</tr>
<?php endif; ?>
</table>