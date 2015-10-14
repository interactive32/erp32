{atkmessages}
{if count($atkmessages)}
<div class="atkmessages">
  {foreach from=$atkmessages item=message}
    <div class="atkmessages_{$message.type}">{$message.message}</div>
  {/foreach}
</div>
{/if}
<div class="actionpageWrapper">

<table border="0" cellpadding="0" cellspacing="0">
<tr>

<!-- left sidebar -->
{if $displayLeftSidebar}
<td valign="top" cellspacing="0" cellpadding="0">

<table class="panel" cellpadding="0" cellspacing="0" height="0" style='table-layout:fixed'> 
<tr>

  <tr> 
    <td class="panelHeader">
	<img src="./atk/themes/steelblue/images/contentheaderleftB.jpg" alt="" align="left">
	<img src="./atk/themes/steelblue/images/contentheaderright.jpg" alt="" align="right">
	</td> 
  </tr> 
  <tr> 
    <td class="panelContent"> 
      <div class="panelContent2"> 
		{$leftbar1}
      </div> 
	  
    </td> 
	
  </tr> 
  <tr> 
   <td class="panelFooter2">
    <img src="./atk/themes/steelblue/images/contentfoot2left.jpg" alt="" align="left">
    <img src="./atk/themes/steelblue/images/contentfoot2right.jpg" alt="" align="right">

   </td>
  </tr> 
  
</tr>


<tr height="10">
<!-- Blank space between panels -->
</tr>

<!-- terms of use -->
<tr>

  <tr> 
    <td class="panelHeader">
	<img src="./atk/themes/steelblue/images/contentheaderleftB.jpg" alt="" align="left">
	<img src="./atk/themes/steelblue/images/contentheaderright.jpg" alt="" align="right">
	</td> 
  </tr> 
  <tr> 
    <td class="panelContent"> 
      <div class="panelContent2"> 
		{$leftbar2}
      </div> 
	  
    </td> 
	
  </tr> 
  <tr> 
    <td class="panelFooter"><img src="./atk/themes/steelblue/images/contentfooterright.jpg" alt="" align="right"></td> 
  </tr> 
  
</tr>





</table>

</td>
{/if}
<!-- left sidebar end -->

<td valign="top">
{foreach from=$blocks item=block}
  {$block}
{/foreach}
</td>
</tr>
</table>
<center>
<br><br>
{$actionfooter}
</center>

</div>