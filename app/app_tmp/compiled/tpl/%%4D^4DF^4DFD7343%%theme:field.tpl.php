<?php /* Smarty version 2.6.11, created on 2013-01-18 01:21:10
         compiled from theme:field.tpl */ ?>
<tr<?php if ($this->_tpl_vars['field']['rowid'] != ""): ?> id="<?php echo $this->_tpl_vars['field']['rowid']; ?>
"<?php endif;  if ($this->_tpl_vars['field']['initial_on_tab'] != 'yes'): ?> style="display: none"<?php endif; ?> class="<?php echo $this->_tpl_vars['field']['tab']; ?>
">
  <?php if (isset ( $this->_tpl_vars['field']['line'] ) && $this->_tpl_vars['field']['line'] != ""): ?>
    <td colspan="2" valign="top" nowrap="nowrap" class="field"><?php echo $this->_tpl_vars['field']['line']; ?>
</td>
  <?php else: ?>
  <?php if ($this->_tpl_vars['field']['label'] !== 'AF_NO_LABEL'): ?><td valign="top" class="<?php if (isset ( $this->_tpl_vars['field']['error'] )): ?>errorlabel<?php else: ?>fieldlabel<?php endif; ?>"><?php if ($this->_tpl_vars['field']['label'] != ""): ?><b><?php echo $this->_tpl_vars['field']['label']; ?>
</b>:  <?php if (isset ( $this->_tpl_vars['field']['obligatory'] )):  echo $this->_tpl_vars['field']['obligatory'];  endif;  endif; ?></td><?php endif; ?>
<!--    </tr>
    <tr<?php if ($this->_tpl_vars['field']['rowid'] != ""): ?> id="<?php echo $this->_tpl_vars['field']['rowid']; ?>
"<?php endif;  if ($this->_tpl_vars['field']['initial_on_tab'] != 'yes'): ?> style="display: none"<?php endif; ?> class="<?php echo $this->_tpl_vars['field']['tab']; ?>
"> -->
    <td valign="top" id="<?php echo $this->_tpl_vars['field']['id']; ?>
" <?php if ($this->_tpl_vars['field']['label'] === 'AF_NO_LABEL'): ?>colspan="2"<?php endif; ?> class="field"><?php echo $this->_tpl_vars['field']['full']; ?>
</td>
  <?php endif; ?>
</tr>