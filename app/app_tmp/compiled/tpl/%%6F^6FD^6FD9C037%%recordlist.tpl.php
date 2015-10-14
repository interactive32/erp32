<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:50
         compiled from ./atk/themes/steelblue/templates/recordlist.tpl */ ?>
<?php if (isset ( $this->_tpl_vars['formstart'] )):  echo $this->_tpl_vars['formstart'];  endif; ?>
<table cellspacing="0" cellpadding="0" class="recordListContainer">
  <tr>
    <td>
        <table id="<?php echo $this->_tpl_vars['listid']; ?>
" class="recordList" cellpadding="0" cellspacing="0">
            <!-- header -->
            <tr>
              <?php unset($this->_sections['headerloop']);
$this->_sections['headerloop']['name'] = 'headerloop';
$this->_sections['headerloop']['loop'] = is_array($_loop=$this->_tpl_vars['header']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['headerloop']['show'] = true;
$this->_sections['headerloop']['max'] = $this->_sections['headerloop']['loop'];
$this->_sections['headerloop']['step'] = 1;
$this->_sections['headerloop']['start'] = $this->_sections['headerloop']['step'] > 0 ? 0 : $this->_sections['headerloop']['loop']-1;
if ($this->_sections['headerloop']['show']) {
    $this->_sections['headerloop']['total'] = $this->_sections['headerloop']['loop'];
    if ($this->_sections['headerloop']['total'] == 0)
        $this->_sections['headerloop']['show'] = false;
} else
    $this->_sections['headerloop']['total'] = 0;
if ($this->_sections['headerloop']['show']):

            for ($this->_sections['headerloop']['index'] = $this->_sections['headerloop']['start'], $this->_sections['headerloop']['iteration'] = 1;
                 $this->_sections['headerloop']['iteration'] <= $this->_sections['headerloop']['total'];
                 $this->_sections['headerloop']['index'] += $this->_sections['headerloop']['step'], $this->_sections['headerloop']['iteration']++):
$this->_sections['headerloop']['rownum'] = $this->_sections['headerloop']['iteration'];
$this->_sections['headerloop']['index_prev'] = $this->_sections['headerloop']['index'] - $this->_sections['headerloop']['step'];
$this->_sections['headerloop']['index_next'] = $this->_sections['headerloop']['index'] + $this->_sections['headerloop']['step'];
$this->_sections['headerloop']['first']      = ($this->_sections['headerloop']['iteration'] == 1);
$this->_sections['headerloop']['last']       = ($this->_sections['headerloop']['iteration'] == $this->_sections['headerloop']['total']);
?>
                <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php if (isset ( $this->_tpl_vars['header'][$this->_sections['headerloop']['index']]['htmlattributes'] )):  echo $this->_tpl_vars['header'][$this->_sections['headerloop']['index']]['htmlattributes'];  endif; ?>
                 <?php if ($this->_sections['headerloop']['index'] === 0): ?>class="recordListThFirst"<?php else: ?>class="recordListTh"<?php endif; ?>>
                  <?php if ($this->_tpl_vars['header'][$this->_sections['headerloop']['index']]['content'] != ""):  echo $this->_tpl_vars['header'][$this->_sections['headerloop']['index']]['content'];  else: ?>&nbsp;<?php endif; ?>
                </th>
              <?php endfor; endif; ?>
            </tr>

            <?php if (count ( $this->_tpl_vars['sort'] )): ?>
              <!-- sort row -->
              <tr>
              <?php echo $this->_tpl_vars['sortstart']; ?>

              <?php $_from = $this->_tpl_vars['sort']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                  <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php if (isset ( $this->_tpl_vars['col']['htmlattributes'] )):  echo $this->_tpl_vars['col']['htmlattributes'];  endif; ?>>
                    <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>
                  </th>
              <?php endforeach; endif; unset($_from); ?>
              <?php echo $this->_tpl_vars['sortend']; ?>

              </tr>
            <?php endif; ?>

            <?php if (count ( $this->_tpl_vars['search'] )): ?>
              <!-- search row -->
              <tr>
              <?php echo $this->_tpl_vars['searchstart']; ?>

              <?php $_from = $this->_tpl_vars['search']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                  <th class="recordListSearch" valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php if (isset ( $this->_tpl_vars['col']['htmlattributes'] )):  echo $this->_tpl_vars['col']['htmlattributes'];  endif; ?>>
                    <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>
                  </th>
              <?php endforeach; endif; unset($_from); ?>
              <?php echo $this->_tpl_vars['searchend']; ?>

              </tr>
            <?php endif; ?>

            <!-- records -->
            <?php echo $this->_tpl_vars['liststart']; ?>

            <?php $_from = $this->_tpl_vars['rows']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['row']):
?>
              <tr id="<?php echo $this->_tpl_vars['row']['id']; ?>
" class="row<?php if ($this->_tpl_vars['row']['rownum'] % 2 == 0): ?>1<?php else: ?>2<?php endif; ?>" <?php if ($this->_tpl_vars['row']['background'] != ""): ?>style="background-color:<?php echo $this->_tpl_vars['row']['background']; ?>
" <?php endif; ?>
                   onmouseover="highlightrow(this, '<?php echo $this->_tpl_vars['row']['highlight']; ?>
')"
                   onmouseout="resetrow(this)"
                   onclick="selectrow(this, '<?php echo $this->_tpl_vars['listid']; ?>
', <?php echo $this->_tpl_vars['row']['rownum']; ?>
)">
               <?php unset($this->_sections['colloop']);
$this->_sections['colloop']['name'] = 'colloop';
$this->_sections['colloop']['loop'] = is_array($_loop=$this->_tpl_vars['row']['cols']) ? count($_loop) : max(0, (int)$_loop); unset($_loop);
$this->_sections['colloop']['show'] = true;
$this->_sections['colloop']['max'] = $this->_sections['colloop']['loop'];
$this->_sections['colloop']['step'] = 1;
$this->_sections['colloop']['start'] = $this->_sections['colloop']['step'] > 0 ? 0 : $this->_sections['colloop']['loop']-1;
if ($this->_sections['colloop']['show']) {
    $this->_sections['colloop']['total'] = $this->_sections['colloop']['loop'];
    if ($this->_sections['colloop']['total'] == 0)
        $this->_sections['colloop']['show'] = false;
} else
    $this->_sections['colloop']['total'] = 0;
if ($this->_sections['colloop']['show']):

            for ($this->_sections['colloop']['index'] = $this->_sections['colloop']['start'], $this->_sections['colloop']['iteration'] = 1;
                 $this->_sections['colloop']['iteration'] <= $this->_sections['colloop']['total'];
                 $this->_sections['colloop']['index'] += $this->_sections['colloop']['step'], $this->_sections['colloop']['iteration']++):
$this->_sections['colloop']['rownum'] = $this->_sections['colloop']['iteration'];
$this->_sections['colloop']['index_prev'] = $this->_sections['colloop']['index'] - $this->_sections['colloop']['step'];
$this->_sections['colloop']['index_next'] = $this->_sections['colloop']['index'] + $this->_sections['colloop']['step'];
$this->_sections['colloop']['first']      = ($this->_sections['colloop']['iteration'] == 1);
$this->_sections['colloop']['last']       = ($this->_sections['colloop']['iteration'] == $this->_sections['colloop']['total']);
?>
               <<?php if ($this->_tpl_vars['row']['type'] == 'subtotal'): ?>th<?php else: ?>td<?php endif; ?>
                    class="<?php if ($this->_sections['colloop']['index'] === 0): ?>recordListTdFirst<?php else: ?>recordListTd<?php endif;  if ($this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['type'] == 'data'): ?> clickable<?php endif; ?>"
                    valign="<?php echo $this->_tpl_vars['vorientation']; ?>
"  <?php if (isset ( $this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['htmlattributes'] )):  echo $this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['htmlattributes'];  endif; ?>
                    <?php if ($this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['type'] == 'data'): ?> onclick="rl_try('<?php echo $this->_tpl_vars['listid']; ?>
', event, <?php echo $this->_tpl_vars['row']['rownum']; ?>
, ['select', 'edit', 'view'], false);"<?php endif; ?>>
                  <?php if ($this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['content'] != ""):  echo $this->_tpl_vars['row']['cols'][$this->_sections['colloop']['index']]['content'];  else: ?>&nbsp;<?php endif; ?>
                </<?php if ($this->_tpl_vars['row']['type'] == 'subtotal'): ?>th<?php else: ?>td<?php endif; ?>>
              <?php endfor; endif; ?>
            </tr>
            <?php endforeach; endif; unset($_from); ?>
            <?php echo $this->_tpl_vars['listend']; ?>


            <?php if (count ( $this->_tpl_vars['total'] )): ?>
            <!-- totals row -->
              <tr>
              <?php $_from = $this->_tpl_vars['total']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['col']):
?>
                  <th valign="<?php echo $this->_tpl_vars['vorientation']; ?>
" <?php if (isset ( $this->_tpl_vars['col']['htmlattributes'] )):  echo $this->_tpl_vars['col']['htmlattributes'];  endif; ?>>
                    <?php if ($this->_tpl_vars['col']['content'] != ""):  echo $this->_tpl_vars['col']['content'];  else: ?>&nbsp;<?php endif; ?>
                  </th>
              <?php endforeach; endif; unset($_from); ?>
              </tr>
            <?php endif; ?>
      </table>
    </td>
  </tr>
  <?php if ($this->_tpl_vars['mra'] != ""): ?>
  <!-- multirecord actions -->
   <tr>
     <td valign="top">
       <?php if ($this->_tpl_vars['editing']): ?>
         <?php echo $this->_tpl_vars['mra']; ?>

       <?php else: ?>
         <table border="0" cellspacing="0" cellpadding="0">
           <tr>
             <td valign="top"><img src="<?php echo $this->_tpl_vars['atkroot']; ?>
atk/images/arrow.gif" border="0"></td>
             <td>
               <?php echo $this->_tpl_vars['mra']; ?>

             </td>
           </tr>
         </table>
       <?php endif; ?>
     </td>
   </tr>
  <?php endif; ?>
</table>
<?php if (isset ( $this->_tpl_vars['formend'] )):  echo $this->_tpl_vars['formend'];  endif; ?>