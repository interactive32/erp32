
<table class="panelcenter" cellpadding="0" cellspacing="0">
  <tr>
  <br><br><br><br>
  </tr>
  <tr>
    <td class="LoginHeader"><img src="{atkthemeimg contentheaderright.jpg}" alt="" align="right"></td>
  </tr>
  <tr>
    <td class="panelContent">
	    {if isset($closebutton)}
		<a href="javascript:window.close();">
		<img src="{atkthemeimg close.gif}" alt="" border=0 align="right" onclick="javascript:window.close();">
		</a>
		{/if}
      <div class="panelContent2">
	  <br>
	  {$content}
      </div>
    </td>
  </tr>
  <tr>
    <td class="panelFooter"><img src="{atkthemeimg contentfooterright.jpg}" alt="" align="right"></td>
  </tr>
</table>
<br class="clearfloat" />
