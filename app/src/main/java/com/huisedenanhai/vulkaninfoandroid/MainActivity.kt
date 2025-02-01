package com.huisedenanhai.vulkaninfoandroid

import android.annotation.SuppressLint
import android.graphics.Color
import android.os.Bundle
import android.text.Spannable
import android.text.SpannableString
import android.text.style.BackgroundColorSpan
import android.text.style.ForegroundColorSpan
import android.view.View
import android.widget.SearchView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.huisedenanhai.vulkaninfoandroid.databinding.ActivityMainBinding

data class SearchSpan(val start: Int, val end: Int)

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var vulkanInfoString: String
    private var searchedSpans: ArrayList<SearchSpan> = ArrayList()

    @SuppressLint("DefaultLocale")
    private fun setHighLightedText(query: String): Boolean {
        val queryTextLowerCase = query.lowercase()
        val wordToSpan = SpannableString(vulkanInfoString)
        val tvt = vulkanInfoString.lowercase()
        var spanStart = tvt.indexOf(queryTextLowerCase, 0)
        var searchStart = 0
        searchedSpans.clear()
        while (searchStart < tvt.length && spanStart != -1) {
            spanStart = tvt.indexOf(queryTextLowerCase, searchStart)
            if (spanStart == -1) break
            else {
                val spanEnd = spanStart + queryTextLowerCase.length
                // set color here
                wordToSpan.setSpan(
                    BackgroundColorSpan(Color.YELLOW),
                    spanStart,
                    spanEnd,
                    Spannable.SPAN_EXCLUSIVE_EXCLUSIVE
                )
                wordToSpan.setSpan(
                    ForegroundColorSpan(Color.BLACK),
                    spanStart,
                    spanEnd,
                    Spannable.SPAN_EXCLUSIVE_EXCLUSIVE
                )
                searchedSpans.add(SearchSpan(spanStart, spanEnd))
            }
            searchStart = spanStart + 1
        }

        if (searchedSpans.isEmpty()) {
            binding.searchInfoText.visibility = View.INVISIBLE
        } else {
            binding.sampleText.setText(wordToSpan, TextView.BufferType.SPANNABLE)
            binding.searchInfoText.visibility = View.VISIBLE
            binding.searchInfoText.text = String.format("Matches found %d", searchedSpans.size)
        }

        return searchedSpans.isNotEmpty()
    }

    private fun clearTextHighLight() {
        searchedSpans.clear()
        binding.searchInfoText.visibility = View.INVISIBLE
        binding.sampleText.text = vulkanInfoString
    }

    private fun updateVulkanInfoString() {
        vulkanInfoString = getVulkanInfoStringJNI()
        clearTextHighLight()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        this.supportActionBar?.hide()

        binding.searchView.setOnQueryTextListener(object : SearchView.OnQueryTextListener {
            override fun onQueryTextSubmit(query: String): Boolean {
                if (!setHighLightedText(query)) {
                    clearTextHighLight()
                    Toast.makeText(this@MainActivity, "No Match found", Toast.LENGTH_LONG).show()
                }
                return false
            }

            override fun onQueryTextChange(newText: String): Boolean {
                if (searchedSpans.isNotEmpty()) {
                    clearTextHighLight()
                }
                return false
            }
        })

        binding.searchView.setOnCloseListener {
            clearTextHighLight()
            false
        }

        updateVulkanInfoString()
    }

    /**
     * A native method that is implemented by the 'vulkaninfoandroid' native library,
     * which is packaged with this application.
     */
    private external fun getVulkanInfoStringJNI(): String

    companion object {
        // Used to load the 'vulkaninfoandroid' library on application startup.
        init {
            System.loadLibrary("vulkaninfoandroid")
        }
    }
}